import argparse
import os
import pathlib
from pathlib import Path

import torch
from eval import eval, write_mask
from load_model import load_model
from torch.utils.tensorboard import SummaryWriter
from final.deeplabv3.train import train

from final.dataloader import get_water_dataloader


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--run_name", type=str, default="deeplabv3-test")
    parser.add_argument("--train", action="store_true")
    parser.add_argument("--eval", action="store_true")
    parser.add_argument("--data_root", type=str, default="/home/user/image_processing/final/training_dataset")
    parser.add_argument("--image_folder", type=str, default="image")
    parser.add_argument("--mask_folder", type=str, default="mask")
    parser.add_argument("--root", type=str, default=os.path.join(Path(__file__).parent.absolute()))
    parser.add_argument("--cpu", action="store_true")
    parser.add_argument("--model", type=str, default="resnet50")
    parser.add_argument("--save_every_epoch", default=50, type=int, help="save every epoch")

    parser.add_argument("--epochs", type=int, default=100)
    parser.add_argument("--batch_size", type=int, default=10)
    parser.add_argument("--lr", type=float, default=0.001)

    return parser.parse_args()


def main():
    args = parse_args()
    # for arg in args.__dict__:
    # print(f"{arg}, {getattr(args, arg)}")
    print(f"Run name: {args.run_name}")

    mode = "train"
    mode = "eval" if args.eval else mode

    print(f"Mode: {mode}")
    if not args.cpu and torch.cuda.is_available():
        device = torch.device("cuda")
        print("Using GPU")
    else:
        device = torch.device("cpu")
        print("Using CPU")
    args.device = device

    # Check if data_root exists
    run_root = Path(args.root) / "runs" / args.run_name
    args.run_root = run_root
    if not os.path.exists(run_root):
        os.makedirs(run_root)
        print(f"Created run root {run_root}")
    else:
        print(f"Run root {run_root} already exists")

    model = load_model(args.model)
    print(f"Model: {args.model}")
    if mode == "train":
        model.train()
        model.to(device)
        dataloader = get_water_dataloader(args.data_root, args.image_folder, args.mask_folder, args.batch_size)
        print(f"{len(dataloader.dataset)} training images loaded.")
        tb_writer = SummaryWriter(log_dir=run_root)
        train(model, dataloader, tb_writer, args)
    else:
        model.eval()


if __name__ == "__main__":
    main()
