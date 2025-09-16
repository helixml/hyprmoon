#!/bin/bash
set -euo pipefail

# Container-side build script for HyprMoon
# This script runs inside the hyprmoon-build-env container

# Create timestamped log file inside container (bind mounted to host)
TIMESTAMP=$(date +%s)
CONTAINER_LOG="/workspace/container-build-${TIMESTAMP}.log"

# Simple logging with tee
exec > >(tee -a "$CONTAINER_LOG") 2>&1

echo "=== HyprMoon Container Build Script ==="
echo "Working directory: $(pwd)"
echo "Build timestamp: $(date)"
echo "Container log file: $CONTAINER_LOG"

# Navigate to source directory
cd /workspace/hyprland-0.41.2+ds

# Update package lists
echo "Updating package lists..."
apt-get update -qq

# Install build dependencies
echo "Installing build dependencies..."
mk-build-deps --install --remove --tool='apt-get -o Debug::pkgProblemResolver=yes --no-install-recommends --yes -qq' debian/control

# Build the deb package with real-time fatal error detection
echo "Building deb package..."

# Stream output line by line and check for fatal errors
dpkg-buildpackage -us -uc -b 2>&1 | while IFS= read -r line; do
    echo "$line"
    if [[ "$line" == *"fatal error:"* ]] || [[ "$line" == *"FATAL"* ]]; then
        echo "=== FATAL ERROR DETECTED - STOPPING BUILD ==="
        echo "Error line: $line"
        exit 1
    fi
done

# Capture the exit code from dpkg-buildpackage (through the pipe)
BUILD_EXIT_CODE=${PIPESTATUS[0]}

# Check if the build failed
if [ $BUILD_EXIT_CODE -ne 0 ]; then
    echo "=== BUILD FAILED (exit code: $BUILD_EXIT_CODE) ==="
    exit $BUILD_EXIT_CODE
fi

echo "=== Build completed successfully ==="
echo "Generated files:"
ls -la /workspace/*.deb 2>/dev/null || echo "No deb files found in /workspace"

# Show package details
if ls /workspace/hyprmoon_*.deb >/dev/null 2>&1; then
    echo "Package details:"
    dpkg-deb --info /workspace/hyprmoon_*.deb | head -10
fi

echo "Container build log saved to: $CONTAINER_LOG"