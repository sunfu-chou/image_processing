import cv2
import numpy as np


def calculate_iou(mask1, mask2):
    intersection = np.logical_and(mask1, mask2)
    union = np.logical_or(mask1, mask2)
    iou = np.sum(intersection) / np.sum(union)
    return iou


num = 12
mask_filenames_1 = [f"/home/user/image_processing/final/demo_data/mask/output{i+1}.jpg" for i in range(num)]
mask_filenames_2 = [f"/home/user/image_processing/final/demo_data/gt/input{i+1}.jpg" for i in range(num)]

total_iou = 0

for i in range(num):
    mask1 = cv2.imread(mask_filenames_1[i], cv2.IMREAD_GRAYSCALE)
    mask2 = cv2.imread(mask_filenames_2[i], cv2.IMREAD_GRAYSCALE)

    _, mask1 = cv2.threshold(mask1, 128, 255, cv2.THRESH_BINARY)
    _, mask2 = cv2.threshold(mask2, 128, 255, cv2.THRESH_BINARY)

    iou = calculate_iou(mask1, mask2)
    total_iou += iou
    print(f"IoU for pair {i + 1:2d}: {iou: .6f}")

average_iou = total_iou / len(mask_filenames_1)
print(f"Average IoU    : {average_iou: .6f}")
