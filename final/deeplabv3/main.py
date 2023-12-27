import torch
from eval import eval, write_mask
from load_model import load_model
from train import train

from final.dataloader import get_water_dataloader

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

model = load_model("resnet50")
model.train()

dataloaders = get_water_dataloader("/home/user/image_processing/final/training_dataset", batch_size=10)

model.to(device)

# train(model, dataloaders, device, 300)
# exit()

model.load_state_dict(torch.load("final/deeplabv3/resnet50.pth"))
model.to(device)
model.eval()

# import numpy as np
# from PIL import Image
# from torchvision.transforms import v2 as transforms

# image_name = "/home/user/image_processing/final/training_dataset/image/1.jpg"
# with open(image_name, "rb") as image_file:
#     image = Image.open(image_file).convert("RGB")
#     data_transforms = transforms.Compose(
#             [
#                 transforms.ToImage(),
#                 transforms.Resize((520, 520), antialias=True),
#                 transforms.ToDtype(torch.float32, scale=True),
#                 transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
#             ]
#         )
#     image = data_transforms(image)
#     image = image.unsqueeze(0)
#     image = image.to(device)
#     output = model(image)['out'].cpu().detach().numpy()[0]*255
#     print(f"Input shape: {image.shape}")
#     print(f"Output shape: {output.shape}")
#     print(f"Output min: {output.min()}")
#     print(f"Output max: {output.max()}")
#     output = output.transpose((1, 2, 0))  # Reshape to have a third dimension of size 3 or 4
#     if output.shape[2] == 1:
#         output = np.repeat(output, 3, axis=2)  # Repeat the single channel to create 3 channels
#     # Save the output mask as an image
#     output_mask = Image.fromarray(output.astype('uint8')).convert('L')  # Convert to RGB mode
#     output_mask.save("final/deeplabv3/output_mask.jpg")

import numpy as np
from PIL import Image

for i in range(60):
    image_path = f"/home/user/image_processing/final/training_dataset/image/{i+1}.jpg"
    mask_path = f"/home/user/image_processing/final/training_dataset/mask/{i+1}.jpg"
    output_mask, mask = eval(model, image_path, mask_path, device)

    write_mask(output_mask, f"final/deeplabv3/output/{i+1}.jpg")
