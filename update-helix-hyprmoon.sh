#!/bin/bash
set -e

echo "=== Updating HyprMoon in Helix Container ==="

# Check if production build exists
if [ ! -f "$HOME/.cache/hyprmoon-build/hyprmoon-prod/src/Hyprland" ]; then
    echo "ERROR: Production HyprMoon binary not found!"
    echo "Expected at: $HOME/.cache/hyprmoon-build/hyprmoon-prod/src/Hyprland"
    exit 1
fi

# Find the running helix container
CONTAINER_ID=$(docker ps | grep helix-hyprmoon | awk '{print $1}')
if [ -z "$CONTAINER_ID" ]; then
    echo "ERROR: No running helix-hyprmoon container found!"
    echo "Available containers:"
    docker ps
    exit 1
fi

echo "Found helix container: $CONTAINER_ID"

# Backup original binary
echo "Backing up original Hyprland binary..."
docker exec $CONTAINER_ID cp /usr/bin/Hyprland /usr/bin/Hyprland.backup 2>/dev/null || echo "Original backup may not exist"

# Copy new binary into container
echo "Copying new HyprMoon binary into container..."
docker cp "$HOME/.cache/hyprmoon-build/hyprmoon-prod/src/Hyprland" $CONTAINER_ID:/usr/bin/Hyprland

# Make it executable
docker exec $CONTAINER_ID chmod +x /usr/bin/Hyprland

echo "HyprMoon binary updated successfully!"
echo "You may need to restart the container or kill existing Hyprland processes"

# Show container status
echo ""
echo "Container status:"
docker exec $CONTAINER_ID ps aux | grep -E "(Hyprland|wayvnc)" || echo "No Hyprland/wayvnc processes found"