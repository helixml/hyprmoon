#!/bin/bash
set -e

# Container-side build script for HyprMoon
# This script runs inside the hyprmoon-build-env container

# Create timestamped log file inside container (bind mounted to host)
TIMESTAMP=$(date +%s)
CONTAINER_LOG="/workspace/container-build-${TIMESTAMP}.log"

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

# Monitor build output for fatal errors and exit immediately
# Create a temporary file to signal fatal error
FATAL_ERROR_FLAG="/tmp/fatal_error_$$"
rm -f "$FATAL_ERROR_FLAG"

# Start build and monitor for fatal errors
{
    dpkg-buildpackage -us -uc -b 2>&1 | while IFS= read -r line; do
        echo "$line"
        if [[ "$line" == *"fatal error:"* ]]; then
            echo "=== FATAL ERROR DETECTED - STOPPING BUILD ==="
            echo "Error line: $line"
            touch "$FATAL_ERROR_FLAG"
            # Kill all build processes
            pkill -f "dpkg-buildpackage" 2>/dev/null || true
            pkill -f "cmake" 2>/dev/null || true
            pkill -f "ninja" 2>/dev/null || true
            exit 1
        fi
    done
    echo $? > /tmp/build_exit_code_$$
} &

BUILD_MONITOR_PID=$!

# Wait for either completion or fatal error
while kill -0 $BUILD_MONITOR_PID 2>/dev/null; do
    if [[ -f "$FATAL_ERROR_FLAG" ]]; then
        echo "=== FATAL ERROR DETECTED - EXITING ==="
        kill $BUILD_MONITOR_PID 2>/dev/null || true
        rm -f "$FATAL_ERROR_FLAG" /tmp/build_exit_code_$$
        exit 1
    fi
    sleep 1
done

# Check final exit code
if [[ -f /tmp/build_exit_code_$$ ]]; then
    BUILD_EXIT_CODE=$(cat /tmp/build_exit_code_$$)
    rm -f /tmp/build_exit_code_$$
else
    BUILD_EXIT_CODE=1
fi

rm -f "$FATAL_ERROR_FLAG"

# Check if the build failed
if [ $BUILD_EXIT_CODE -ne 0 ]; then
    echo "=== BUILD FAILED ==="
    exit 1
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