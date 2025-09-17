#!/bin/bash
set -euo pipefail

# Container-side build script for HyprMoon
# This script runs inside the hyprmoon-build-env container

# Container build with proper logging and metrics
BUILD_START=$(date +%s)
CONTAINER_LOG="/workspace/container-build-${BUILD_START}.log"
METRICS_CSV="/workspace/build-metrics.csv"

# Configure ccache to use mounted directory
export CCACHE_DIR=/ccache
export PATH="/usr/lib/ccache:$PATH"

# Initialize CSV header if it doesn't exist
if [ ! -f "$METRICS_CSV" ]; then
    echo "timestamp,version,duration_seconds,ccache_hit_rate_before,ccache_files_before,ccache_size_before,ccache_hit_rate_after,ccache_files_after,ccache_size_after,ninja_targets_before,ninja_cache_files_before,ninja_targets_after,ninja_object_files_after" > "$METRICS_CSV"
fi

# Function to extract ccache stats
get_ccache_stats() {
    # Set ccache directory to the mounted location
    export CCACHE_DIR=/ccache
    local stats=$(ccache -s 2>/dev/null)
    local hit_rate=$(echo "$stats" | grep "Hits:" | head -1 | sed 's/.*(\([0-9.]*\)%).*/\1/' || echo "0")
    local files=$(echo "$stats" | grep "files in cache" | sed 's/.*: *\([0-9]*\).*/\1/' || echo "0")
    local size=$(echo "$stats" | grep "Cache size" | sed 's/.*: *\([0-9.]*\).*/\1/' || echo "0")
    echo "${hit_rate},${files},${size}"
}

# Function to get ninja stats
get_ninja_stats() {
    local targets=0
    local cache_files=0
    if [ -d build ]; then
        targets=$(cd build && ninja -t targets all 2>/dev/null | wc -l || echo "0")
        cache_files=$(cd build && find . -name "*.ninja*" 2>/dev/null | wc -l || echo "0")
    fi
    echo "${targets},${cache_files}"
}

# Redirect output to both console and log file, but handle SIGPIPE gracefully
exec > >(tee -a "$CONTAINER_LOG") 2>&1
trap '' PIPE  # Ignore SIGPIPE to prevent exit code 141

echo "=== HyprMoon Container Build Script ==="
echo "Working directory: $(pwd)"
echo "Build timestamp: $(date)"
echo "Container log file: $CONTAINER_LOG"

# Navigate to source directory
cd /workspace/hyprland-0.41.2+ds

# Update package lists
echo "Updating package lists..."
apt-get update -qq

# Install build dependencies (keep dummy package to avoid reinstalls)
echo "Installing build dependencies..."
mk-build-deps --install --tool='apt-get -o Debug::pkgProblemResolver=yes --no-install-recommends --yes -qq' debian/control

# Get version from changelog
VERSION=$(grep -m1 '^hyprmoon (' /workspace/hyprland-0.41.2+ds/debian/changelog | sed 's/hyprmoon (\([^)]*\)).*/\1/')

# Collect metrics before build
echo "=== COLLECTING BUILD METRICS ==="
CCACHE_BEFORE=$(get_ccache_stats)
NINJA_BEFORE=$(get_ninja_stats)
echo "Pre-build - ccache: $CCACHE_BEFORE, ninja: $NINJA_BEFORE"

# Build the deb package
echo "Building deb package..."

# Show ccache stats before build
echo "=== CCACHE STATS BEFORE BUILD ==="
CCACHE_DIR=/ccache ccache -s | head -10

# Use incremental build approach for speed
if [ -d build ] && [ -f build/.ninja_log ]; then
    BUILD_TYPE="INCREMENTAL"
    echo "Detected existing build directory - using fast incremental build"
    if ! fakeroot debian/rules binary; then
        echo "=== INCREMENTAL BUILD FAILED ==="
        echo "fakeroot debian/rules binary failed with exit code $?"
        exit 1
    fi
else
    BUILD_TYPE="FULL"
    echo "No existing build detected - using full dpkg-buildpackage"
    if ! dpkg-buildpackage -us -uc -b; then
        echo "=== FULL BUILD FAILED ==="
        echo "dpkg-buildpackage failed with exit code $?"
        exit 1
    fi
fi

echo "dpkg-buildpackage completed successfully"

# Collect metrics after build
BUILD_END=$(date +%s)
DURATION=$((BUILD_END - BUILD_START))
CCACHE_AFTER=$(get_ccache_stats)
NINJA_AFTER=$(get_ninja_stats)

# Get ninja object file count for after stats
NINJA_OBJECTS=0
if [ -d build ]; then
    NINJA_OBJECTS=$(cd build && find . -name "*.o" 2>/dev/null | wc -l || echo "0")
fi

echo "Post-build - duration: ${DURATION}s, ccache: $CCACHE_AFTER, ninja: $NINJA_AFTER, objects: $NINJA_OBJECTS"

# Write metrics to CSV (ensure single line)
printf "%s,%s,%s,%s,%s,%s,%s\n" "${BUILD_START}" "${VERSION}" "${DURATION}" "${CCACHE_BEFORE}" "${CCACHE_AFTER}" "${NINJA_BEFORE}" "${NINJA_OBJECTS}" >> "$METRICS_CSV"

# Show ccache stats after build
echo "=== CCACHE STATS AFTER BUILD ==="
CCACHE_DIR=/ccache ccache -s | head -15

echo "=== Build completed successfully ==="
echo "Generated files:"
ls -la /workspace/*.deb 2>/dev/null || echo "No deb files found in /workspace"

# Show package details
if ls /workspace/hyprmoon_*.deb >/dev/null 2>&1; then
    echo "Package details:"
    set +e
    set +o pipefail
    for deb in /workspace/hyprmoon_*.deb; do
        if [ -f "$deb" ]; then
            echo "=== $(basename "$deb") ==="
            dpkg-deb --info "$deb" | head -5
            echo ""
        fi
    done
    set -e
    set -o pipefail
fi

echo "Container build log saved to: $CONTAINER_LOG"

# Explicit successful exit
echo "=== CONTAINER BUILD SCRIPT COMPLETED SUCCESSFULLY ==="
echo "Exiting with code 0"
exit 0