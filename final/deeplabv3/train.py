import numpy as np
import torch
from PIL import Image
from torchvision.transforms import v2 as transforms
from tqdm import tqdm


def train(model, dataloaders, tb_writer, args):
    critierion = torch.nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=args.lr)

    for epoch in tqdm(range(args.epochs)):
        pbar = tqdm(enumerate(dataloaders), total=len(dataloaders), leave=False)
        total_loss = 0
        model.train()
        for i, (images, masks) in pbar:
            images = images.to(args.device)
            masks = masks.to(args.device).squeeze(1).long()
            optimizer.zero_grad()
            with torch.set_grad_enabled(True):
                output = model(images)
                loss = critierion(output["out"], masks)
                loss.backward()
                optimizer.step()

            # pbar.set_postfix_str(f"Epoch: {epoch}, Batch: {i}, Loss: {loss.item()}")

            total_loss += loss.item()
        if epoch % args.save_every_epoch == 0:
            torch.save(model.state_dict(), args.run_root / f"{args.model}_{epoch}.pth")
        model.eval()
        image_name = "/home/user/image_processing/final/training_dataset/image/1.jpg"
        with open(image_name, "rb") as image_file:
            image = Image.open(image_file).convert("RGB")
            data_transforms = transforms.Compose(
                [
                    transforms.ToImage(),
                    transforms.Resize((520, 520), antialias=True),
                    transforms.ToDtype(torch.float32, scale=True),
                    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
                ]
            )
            image = data_transforms(image).unsqueeze(0).to(args.device)
            output = model(image)
        output_mask = output["out"].detach().cpu().numpy()[0] * 255
        output_mask = output_mask.transpose((1, 2, 0))

        mask_name = "/home/user/image_processing/final/training_dataset/mask/1.jpg"
        with open(mask_name, "rb") as mask_file:
            mask = Image.open(mask_file).convert("L")
            data_transforms = transforms.Compose(
                [
                    transforms.ToImage(),
                    transforms.Resize((520, 520), antialias=True),
                    transforms.ToDtype(torch.float32, scale=True),
                ]
            )
            mask = data_transforms(mask)
        tb_writer.add_image("Mask/Background", output_mask[:, :, 0], epoch, dataformats="HW")
        tb_writer.add_image("Mask/Target", output_mask[:, :, 1], epoch, dataformats="HW")
        tb_writer.add_image("Mask/GroundTruth", mask, epoch, dataformats="CHW")
        tb_writer.add_scalar("Loss/train", total_loss / len(dataloaders), epoch)
    torch.save(model.state_dict(), args.run_root / f"{args.model}.pth")
    tb_writer.close()
