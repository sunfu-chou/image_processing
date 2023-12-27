from pathlib import Path

import torch
from dataset import WaterDataset
from torch.utils.data import DataLoader


def collate_fn(batch):
    return tuple(zip(*batch))


def get_water_dataloader(data_dir: str, image_folder: str = "image", mask_folder: str = "mask", batch_size: int = 5):
    water_dataset = WaterDataset(data_dir, image_folder, mask_folder)
    dataloader = DataLoader(water_dataset, batch_size=batch_size, shuffle=True, num_workers=8)
    # TODO: Add random split for train and test
    return dataloader
