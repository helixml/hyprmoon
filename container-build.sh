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
    echo "timestamp,version,duration_seconds,ccache_hit_rate_before,ccache_files_before,ccache_size_before,ccache_hit_rate_after,ccache_files_after,ccache_size_after,ninja_targets_before,ninja_cache_files_before,ninja_targets_after,ninja_object_files_after,hyprland_binary_md5,git_commit_hash,git_changed_files,build_type,deployed_deb_version,deployed_binary_version" > "$METRICS_CSV"
fi

# Function to extract ccache stats
get_ccache_stats() {
    # Set ccache directory to the mounted location
    export CCACHE_DIR=/ccache
    local stats=$(ccache -s 2>/dev/null)
    local hit_rate=$(echo "$stats" | grep "Hits:" | head -1 | sed 's/.*(\([0-9.]*\)%).*/\1/' | tr -d '\n' || echo "0")
    local files=$(echo "$stats" | grep "files in cache" | sed 's/.*: *\([0-9]*\).*/\1/' | tr -d '\n' || echo "0")
    local size=$(echo "$stats" | grep "Cache size" | sed 's/.*: *\([0-9.]*\).*/\1/' | tr -d '\n' || echo "0")
    echo -n "${hit_rate},${files},${size}"
}

# Function to get ninja stats
get_ninja_stats() {
    local targets=0
    local cache_files=0
    if [ -d build ]; then
        targets=$(cd build && ninja -t targets all 2>/dev/null | wc -l | tr -d '\n' || echo "0")
        cache_files=$(cd build && find . -name "*.ninja*" 2>/dev/null | wc -l | tr -d '\n' || echo "0")
    fi
    echo -n "${targets},${cache_files}"
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

# Get version from changelog
VERSION=$(grep -m1 '^hyprmoon (' /workspace/hyprland-0.41.2+ds/debian/changelog | sed 's/hyprmoon (\([^)]*\)).*/\1/')

# CRITICAL: Check for incremental build BEFORE any Debian commands that might wipe build directory
echo "🔍 DEBUG: Checking for incremental build files (BEFORE any cleaning)..."
echo "  build dir exists: $([ -d build ] && echo YES || echo NO)"
echo "  ninja log exists: $([ -f build/.ninja_log ] && echo YES || echo NO)"
echo "  build.ninja exists: $([ -f build/build.ninja ] && echo YES || echo NO)"

if [ -d build ] && [ -f build/.ninja_log ] && [ -f build/build.ninja ]; then
    BUILD_TYPE="INCREMENTAL"
    echo "🚀 INCREMENTAL BUILD: Using direct ninja + ccache"

    # All dependencies pre-installed in enhanced container - skip installation
    echo "INCREMENTAL MODE: Using pre-installed dependencies from enhanced container"

    # Collect metrics before build
    echo "=== COLLECTING BUILD METRICS ==="
    CCACHE_BEFORE=$(get_ccache_stats)
    NINJA_BEFORE=$(get_ninja_stats)
    echo "Pre-build - ccache: $CCACHE_BEFORE, ninja: $NINJA_BEFORE"

    # Show ccache stats before build
    echo "=== CCACHE STATS BEFORE BUILD ==="
    CCACHE_DIR=/ccache ccache -s | head -10

    # Configure ccache and build directly
    export CCACHE_DIR=/ccache
    export PATH="/usr/lib/ccache:$PATH"
    export CC="gcc"
    export CXX="g++"

    # Run ninja directly for incremental compilation
    cd build

    # Fix CMake compiler cache issue for ccache
    echo "Fixing CMake compiler paths for ccache..."
    cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .

    if ! ninja; then
        echo "=== NINJA BUILD FAILED ==="
        exit 1
    fi

    # Live detection testing complete - sleep removed for production use

    cd ..

    # Copy binary to workspace with version suffix and create version file
    if [ -f build/Hyprland ]; then
        cp build/Hyprland "/workspace/Hyprland-${VERSION}"
        echo "${VERSION}" > "/workspace/HYPRMOON_VERSION.txt"
        echo "✓ Binary built and copied: Hyprland-${VERSION}"
        echo "✓ Version file created: HYPRMOON_VERSION.txt"
    else
        echo "❌ ERROR: Hyprland binary not found after ninja build"
        exit 1
    fi

else
    BUILD_TYPE="FULL"
    echo "🔧 FULL BUILD: Using standard dpkg-buildpackage"

    # Update package lists for full builds
    echo "Updating package lists for full build..."
    apt-get update -qq

    # Install dependencies for full builds
    echo "Checking build dependencies..."
    if ! dpkg-checkbuilddeps debian/control >/dev/null 2>&1; then
        echo "Missing dependencies detected - installing via mk-build-deps"
        mk-build-deps --install --tool='apt-get -o Debug::pkgProblemResolver=yes --no-install-recommends --yes -qq' debian/control
    else
        echo "All build dependencies satisfied - skipping mk-build-deps"
    fi

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

# Get Hyprland binary MD5 for code change tracking
HYPRLAND_MD5="unknown"
if [ -f build/Hyprland ]; then
    HYPRLAND_MD5=$(md5sum build/Hyprland | cut -d' ' -f1 | cut -c1-16)
elif [ -f debian/tmp/usr/bin/Hyprland ]; then
    HYPRLAND_MD5=$(md5sum debian/tmp/usr/bin/Hyprland | cut -d' ' -f1 | cut -c1-16)
fi

# Get git info for build tracking (fix ownership issues)
GIT_COMMIT=$(cd /workspace && git config --global --add safe.directory /workspace 2>/dev/null; git rev-parse --short HEAD 2>/dev/null | tr -d '\n' || echo -n "unknown")
GIT_CHANGED_FILES=$(cd /workspace && git status --porcelain 2>/dev/null | wc -l | tr -d '\n' || echo -n "0")

# Get deployment info (written by build.sh during auto-deployment)
DEPLOY_MODE="unknown"
DEB_VERSION="unknown"
BINARY_VERSION="unknown"
if [ -f /workspace/deployment-info.env ]; then
    source /workspace/deployment-info.env
fi

echo "Post-build - duration: ${DURATION}s, ccache: $CCACHE_AFTER, ninja: $NINJA_AFTER, objects: $NINJA_OBJECTS, binary_md5: $HYPRLAND_MD5, git: $GIT_COMMIT, changed: $GIT_CHANGED_FILES, deploy: $DEPLOY_MODE"

# Write metrics to CSV (ensure single line, strip any newlines)
{
    echo -n "${BUILD_START},${VERSION},${DURATION},"
    echo -n "${CCACHE_BEFORE},"
    echo -n "${CCACHE_AFTER},"
    echo -n "${NINJA_BEFORE},"
    echo -n "${NINJA_OBJECTS},${HYPRLAND_MD5},${GIT_COMMIT},${GIT_CHANGED_FILES},${BUILD_TYPE},${DEB_VERSION},${BINARY_VERSION}"
    echo ""
} >> "$METRICS_CSV"

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