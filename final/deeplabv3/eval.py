from pathlib import Path

import numpy as np
import torch
from PIL import Image
from torchvision.transforms import v2 as transforms


def preprocess_rgb(image_path, transform):
    image_size = (0, 0)
    with open(image_path, "rb") as image_file:
        image = Image.open(image_file).convert("RGB")
        image_size = image.size
        image = transform(image)
        image = image.unsqueeze(0)
    return image, image_size


def eval(model, image_path, mask_path, device):
    model.eval()
    transform = transforms.Compose(
        [
            transforms.ToImage(),
            transforms.Resize((520, 520), antialias=True),
            transforms.ToDtype(torch.float32, scale=True),
            transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
        ]
    )
    image, image_size = preprocess_rgb(image_path, transform)
    image = image.to(device)
    output = model(image)
    output_mask = output["out"].detach().cpu().numpy()[0].transpose((1, 2, 0))
    data_transforms = transforms.Compose(
        [
            transforms.ToImage(),
            transforms.Resize(image_size[::-1], antialias=True),
            transforms.ToDtype(torch.float32, scale=True),
        ]
    )
    output_mask = data_transforms(output_mask)
    write_mask(output_mask, mask_path)
    write_blend(image_path, mask_path)
    return output_mask


def eval_all(model, image_folder, mask_folder, device):
    model.eval()
    image_paths = sorted(Path(image_folder).glob("*.jpg"))
    mask_paths = [Path(mask_folder) / f"output{path.stem[5:]}.jpg" for path in image_paths]
    for image_path, mask_path in zip(image_paths, mask_paths):
        print(f"image: {image_path}, mask: {mask_path}")
        eval(model, image_path, mask_path, device)


def write_mask(output_mask, image_path):
    print(f"Output shape: {output_mask.shape}")
    target_mask = output_mask[1, :, :]
    binary_mask = np.uint8(((target_mask - target_mask.min()) / (target_mask.max() - target_mask.min())) > 0.5) * 255
    binary_image = Image.fromarray(binary_mask)
    binary_image.save(image_path)


def write_blend(image_path, mask_path):
    image = Image.open(image_path)
    mask = Image.open(mask_path)
    print(f"Image shape: {image.size}, mask shape: {mask.size}")
    image = image.convert("RGBA")
    mask = mask.convert("RGBA")
    blend = Image.blend(image, mask, 0.5)
    mask_path = str(mask_path)
    mask_path = mask_path.replace("output", "outputblend")
    mask_path = mask_path.replace("jpg", "png")
    blend.save(mask_path)
