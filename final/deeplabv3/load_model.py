from torchvision.models.segmentation import (
    DeepLabV3_MobileNet_V3_Large_Weights,
    DeepLabV3_ResNet50_Weights,
    DeepLabV3_ResNet101_Weights,
    deeplabv3_mobilenet_v3_large,
    deeplabv3_resnet50,
    deeplabv3_resnet101,
)
from torchvision.models.segmentation.deeplabv3 import DeepLabHead


def load_model(model_name: str, classes: int = 2):
    if model_name.lower() not in ["resnet50", "resnet101", "mobilenetv3"]:
        raise ValueError("Invalid model name, must be one of resnet50, resnet101, mobilenetv3")

    if model_name.lower() == "resnet50":
        model = deeplabv3_resnet50(weights=DeepLabV3_ResNet50_Weights.DEFAULT)
    elif model_name.lower() == "resnet101":
        model = deeplabv3_resnet101(weights=DeepLabV3_ResNet101_Weights.DEFAULT)
    elif model_name.lower() == "mobilenetv3":
        model = deeplabv3_mobilenet_v3_large(weights=DeepLabV3_MobileNet_V3_Large_Weights.DEFAULT)

    model.classifier = DeepLabHead(2048, classes)

    return model
