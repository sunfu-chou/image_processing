FROM nvidia/cuda:12.0.1-cudnn8-devel-ubuntu20.04

ARG DEBIAN_FRONTEND=noninteractive

ENV USER=user \
    HOME=/home/user \
    UID=1000 \
    GID=1000

RUN groupadd -g ${GID} ${USER} && \
    useradd -m -s /bin/bash -u ${UID} -g ${GID} -o -c "" ${USER}

RUN apt-get update && \
    apt-get install -q -y --no-install-recommends \
    git \
    vim \
    less \
    curl \
    wget \
    sudo \
    python3-pip

RUN python3 -m pip install -U pip setuptools wheel
RUN pip install opencv-python-headless pycocotools matplotlib onnxruntime onnx
RUN pip install torch==2.1.1 torchvision==0.16.1 torchaudio==2.1.1 --index-url https://download.pytorch.org/whl/cu121
RUN pip install tqdm
RUN apt-get update && \
    apt-get install -q -y --no-install-recommends \
    python3-tk
RUN pip install tensorboard click
COPY ./.bashrc ${HOME}/.bashrc

ENV PATH=${HOME}/.local/bin:${PATH}

USER ${USER}

WORKDIR ${HOME}
