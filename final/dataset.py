from pathlib import Path
from typing import Any, Callable, Optional

import numpy as np
import torch
from PIL import Image
from torchvision.datasets.vision import VisionDataset
from torchvision.transforms import v2 as transforms


class WaterDataset(VisionDataset):
    def __init__(
        self,
        root: str,
        image_folder: str,
        mask_folder: str,
    ) -> None:
        image_folder_path = Path(root) / image_folder
        mask_folder_path = Path(root) / mask_folder
        self.image_names = sorted(image_folder_path.glob("*"), key=lambda x: int(x.stem.split("_")[-1]))
        self.mask_names = sorted(mask_folder_path.glob("*"), key=lambda x: int(x.stem.split("_")[-1]))

        self.image_transforms = transforms.Compose(
            [
                transforms.ToImage(),
                transforms.Resize((520, 520), antialias=True),
                transforms.ToDtype(torch.float32, scale=True),
                transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
            ]
        )
        self.mask_transforms = transforms.Compose(
            [
                transforms.ToImage(),
                transforms.Resize((520, 520), antialias=True),
                transforms.ToDtype(torch.float32, scale=True),
            ]
        )
        self.image = []
        self.mask = []
        for image_name, mask_name in zip(self.image_names, self.mask_names):
            with open(image_name, "rb") as image_file, open(mask_name, "rb") as mask_file:
                image = Image.open(image_file).convert("RGB")
                mask = Image.open(mask_file).convert("L")

                image = self.image_transforms(image)
                mask = self.mask_transforms(mask)

                self.image.append(image)
                self.mask.append(mask)

    def __len__(self) -> int:
        return len(self.image_names)

    def __getitem__(self, index: int) -> Any:
        image = self.image[index]
        mask = self.mask[index]
        return image, mask
