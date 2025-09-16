#!/bin/bash
set -e

echo "Starting HyprMoon development container..."

# Ensure build cache directory exists
mkdir -p ~/.cache/hyprmoon-build

# Check if source exists
if [ ! -f "$(pwd)/meson.build" ]; then
    echo "ERROR: Must run from Hyprland-wlroots source directory"
    exit 1
fi

echo "Running development container with bind mounts:"
echo "- Source: $(pwd) -> /workspace/hyprmoon-src"
echo "- Build cache: ~/.cache/hyprmoon-build -> /workspace/build-cache"

# Run development container with bind mounts
docker run -it --rm \
    --name hyprmoon-dev \
    -v "$(pwd):/workspace/hyprmoon-src" \
    -v "~/.cache/hyprmoon-build:/workspace/build-cache" \
    --privileged \
    --device /dev/dri \
    -e DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    hyprmoon-dev /bin/bash

echo "Development container exited"