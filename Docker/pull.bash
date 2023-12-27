#!/usr/bin/env bash

REPOSITORY="sunfu-chou/nycu-dip-2023-fall"
TAG="12.0.1-cudnn8-devel-ubuntu20.04"

IMG="${REPOSITORY}:${TAG}"

docker pull "${IMG}"
