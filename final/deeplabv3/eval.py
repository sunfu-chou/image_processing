import numpy as np
import torch
from PIL import Image
from torchvision.transforms import v2 as transforms


def preprocess_rgb(image_path, transform):
    with open(image_path, "rb") as image_file:
        image = Image.open(image_file).convert("RGB")
        image = transform(image)
        image = image.unsqueeze(0)
    return image

def eval(model, image_path, mask_path, device):
    model.eval()
    image = preprocess_rgb(image_path, transform).to(device)
    output = model(image)
    output_mask = output["out"].detach().cpu().numpy()[0]
    data_transforms = transforms.Compose(
        [
            transforms.ToImage(),
            transforms.Resize(image_size[::-1], antialias=True),
            transforms.ToDtype(torch.float32, scale=True),
        ]
    )
    output_mask = output_mask.transpose((1, 2, 0))
    print(f"Output shape: {output_mask.shape}")
    output_mask = data_transforms(output_mask)
    # print(f"Output shape: {output_mask.shape}")
    print(f"image size: {image_size}")

    with open(mask_path, "rb") as mask_file:
        mask = Image.open(mask_file).convert("L")
        data_transforms = transforms.Compose(
            [
                transforms.ToImage(),
                transforms.Resize((520, 520), antialias=True),
                transforms.ToDtype(torch.float32, scale=True),
            ]
        )
        mask = data_transforms(mask)
    return output_mask, mask


def write_mask(output_mask, image_path):
    print(f"Output shape: {output_mask.shape}")
    target_mask = output_mask[1, :, :]
    binary_mask = np.uint8(((target_mask - target_mask.min()) / (target_mask.max() - target_mask.min())) > 0.5) * 255
    binary_image = Image.fromarray(binary_mask)
    binary_image.save(image_path)
