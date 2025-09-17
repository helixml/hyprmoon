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

# Build the deb package
echo "Building deb package..."

# Simple approach - run dpkg-buildpackage directly and check exit code
if ! dpkg-buildpackage -us -uc -b; then
    echo "=== BUILD FAILED ==="
    echo "dpkg-buildpackage failed with exit code $?"
    exit 1
fi

echo "dpkg-buildpackage completed successfully"

echo "=== Build completed successfully ==="
echo "Generated files:"
ls -la /workspace/*.deb 2>/dev/null || echo "No deb files found in /workspace"

# Show package details
if ls /workspace/hyprmoon_*.deb >/dev/null 2>&1; then
    echo "Package details:"
    for deb in /workspace/hyprmoon_*.deb; do
        if [ -f "$deb" ]; then
            echo "=== $(basename "$deb") ==="
            dpkg-deb --info "$deb" | head -5
            echo ""
        fi
    done
fi

echo "Container build log saved to: $CONTAINER_LOG"

# Explicit successful exit
echo "=== CONTAINER BUILD SCRIPT COMPLETED SUCCESSFULLY ==="
echo "Exiting with code 0"
exit 0