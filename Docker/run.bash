#!/usr/bin/env bash

ARGS=("$@")

REPOSITORY="sunfu-chou/nycu-dip-2023-fall"
TAG="12.0.1-cudnn8-devel-ubuntu20.04"

IMG="${REPOSITORY}:${TAG}"

USER_NAME="user"
REPO_NAME="image_processing"
CONTAINER_NAME="image_processing-12.0.1-cudnn8-devel-ubuntu20.04"

CONTAINER_ID=$(docker ps -aqf "ancestor=${IMG}")
if [ $CONTAINER_ID ]; then
  echo "Attach to docker container $CONTAINER_ID"
  xhost +
  docker exec --privileged -e DISPLAY=${DISPLAY} -e LINES="$(tput lines)" -it ${CONTAINER_ID} bash
  xhost -
  return
fi

# Make sure processes in the container can connect to the x server
# Necessary so gazebo can create a context for OpenGL rendering (even headless)
XAUTH=/tmp/.docker.xauth
if [ ! -f $XAUTH ]; then
  xauth_list=$(xauth nlist $DISPLAY)
  xauth_list=$(sed -e 's/^..../ffff/' <<<"$xauth_list")
  if [ ! -z "$xauth_list" ]; then
    echo "$xauth_list" | xauth -f $XAUTH nmerge -
  else
    touch $XAUTH
  fi
  chmod a+r $XAUTH
fi

# Prevent executing "docker run" when xauth failed.
if [ ! -f $XAUTH ]; then
  echo "[$XAUTH] was not properly created. Exiting..."
  exit 1
fi

docker run \
  -it \
  --rm \
  --runtime=nvidia \
  -e DISPLAY \
  -e XAUTHORITY=$XAUTH \
  -e HOME=/home/${USER_NAME} \
  -e OPENAI_API_KEY=$OPENAI_API_KEY \
  -e NVIDIA_DRIVER_CAPABILITIES=all \
  -e "TERM=xterm-256color" \
  -v "$XAUTH:$XAUTH" \
  -v "/home/${USER}/${REPO_NAME}:/home/${USER_NAME}/${REPO_NAME}" \
  -v "/home/${USER}/${REPO_NAME}/Docker/.bash_history:/home/${USER_NAME}/.bash_history" \
  -v "/tmp/.X11-unix:/tmp/.X11-unix" \
  -v "/etc/localtime:/etc/localtime:ro" \
  -v "/dev:/dev" \
  -v "/var/run/docker.sock:/var/run/docker.sock" \
  --user "1000:1000" \
  --workdir "/home/${USER_NAME}/${REPO_NAME}" \
  --name "${CONTAINER_NAME}" \
  --network host \
  --privileged \
  --security-opt seccomp=unconfined \
  "${IMG}" \
  bash

