#!/bin/bash
set -e

# Host-side trigger script for HyprMoon builds
# Usage: ./build.sh
# Usage: FORCE_CLEAN=1 ./build.sh (to force clean build directory)

echo "=== HyprMoon Build Trigger ==="
echo "Starting build at $(date)"

# Navigate to hyprmoon directory
cd /home/luke/pm/hyprmoon

# Handle FORCE_CLEAN=1 to remove stale build cache
if [ "$FORCE_CLEAN" = "1" ]; then
    echo "FORCE_CLEAN=1 detected - removing build directory to ensure clean build"
    if [ -d "hyprland-0.41.2+ds/build" ]; then
        echo "Removing hyprland-0.41.2+ds/build directory..."
        rm -rf hyprland-0.41.2+ds/build
        echo "Build directory cleaned successfully"
    else
        echo "Build directory doesn't exist, nothing to clean"
    fi
fi

# Step 8.6: Re-enable ccache for faster builds
mkdir -p /home/luke/.ccache

# Capture current Hyprland binary info for stale build detection (cross-version)
CURRENT_VERSION=$(grep -m1 '^hyprmoon (' hyprland-0.41.2+ds/debian/changelog | sed 's/hyprmoon (\([^)]*\)).*/\1/')
EXPECTED_DEB="hyprmoon_${CURRENT_VERSION}_amd64.deb"
PRE_BUILD_BINARY_MD5=""
PREVIOUS_DEB_FILE=""

# Find the most recent existing deb file (any version) for cross-version comparison
if ls hyprmoon_*.deb >/dev/null 2>&1; then
    PREVIOUS_DEB_FILE=$(ls -t hyprmoon_*.deb | head -1)
    echo "Pre-build: Found existing deb $PREVIOUS_DEB_FILE - extracting Hyprland binary for comparison"

    # Extract binary to /tmp for comparison
    TEMP_DIR=$(mktemp -d -t hyprmoon-stale-check-XXXXXX)
    trap "rm -rf $TEMP_DIR" EXIT

    cd "$TEMP_DIR"
    dpkg-deb -x "/home/luke/pm/hyprmoon/$PREVIOUS_DEB_FILE" .
    if [ -f "usr/bin/Hyprland" ]; then
        PRE_BUILD_BINARY_MD5=$(md5sum usr/bin/Hyprland | cut -d' ' -f1)
        PRE_BUILD_BINARY_SIZE=$(stat -c%s usr/bin/Hyprland)
        echo "Pre-build: Previous Hyprland binary from $PREVIOUS_DEB_FILE"
        echo "Pre-build: Binary size: $PRE_BUILD_BINARY_SIZE bytes, md5: ${PRE_BUILD_BINARY_MD5:0:16}..."
    else
        echo "Pre-build: Warning - could not extract Hyprland binary from $PREVIOUS_DEB_FILE"
    fi
    cd - >/dev/null
    rm -rf "$TEMP_DIR"
    trap - EXIT
else
    echo "Pre-build: No existing hyprmoon deb files found - this is a clean first build"
fi

# Run the container with the bind-mounted build script and ccache
echo "Executing container build..."
echo "Build logs will be saved to: container-build-*.log"
docker run --rm \
    -v $(pwd):/workspace \
    -v /home/luke/.ccache:/ccache \
    hyprmoon-build-env \
    /workspace/container-build.sh

# Capture docker exit code
DOCKER_EXIT_CODE=$?

# Check if build succeeded based on docker exit code
if [ $DOCKER_EXIT_CODE -eq 0 ]; then
    echo "=== Build SUCCESS ==="
    echo "Generated deb file(s):"
    ls -la hyprmoon_*.deb

    # Stale build detection - compare binaries across versions to detect cache issues
    if [ -n "$PRE_BUILD_BINARY_MD5" ] && [ -f "$EXPECTED_DEB" ]; then
        echo "Post-build: Extracting new Hyprland binary for stale build detection"
        # Extract new binary to /tmp for comparison
        TEMP_DIR=$(mktemp -d -t hyprmoon-stale-check-XXXXXX)
        trap "rm -rf $TEMP_DIR" EXIT

        cd "$TEMP_DIR"
        dpkg-deb -x "/home/luke/pm/hyprmoon/$EXPECTED_DEB" .
        if [ -f "usr/bin/Hyprland" ]; then
            POST_BUILD_BINARY_MD5=$(md5sum usr/bin/Hyprland | cut -d' ' -f1)
            POST_BUILD_BINARY_SIZE=$(stat -c%s usr/bin/Hyprland)

            if [ "$PRE_BUILD_BINARY_MD5" = "$POST_BUILD_BINARY_MD5" ]; then
                echo ""
                echo "🚨 === STALE BUILD CACHE DETECTED ==="
                echo "❌ The compiled Hyprland binary is IDENTICAL to the previous version!"
                echo "   Previous: $PREVIOUS_DEB_FILE"
                echo "   Current:  $EXPECTED_DEB"
                echo "   Binary:   usr/bin/Hyprland"
                echo "   Size:     $POST_BUILD_BINARY_SIZE bytes"
                echo "   MD5:      ${POST_BUILD_BINARY_MD5:0:16}..."
                echo ""
                echo "This means your code changes are NOT being compiled into the binary."
                echo "The build cache is stale and needs to be cleared."
                echo ""
                echo "🔧 SOLUTION: Run a clean build to force recompilation:"
                echo "   FORCE_CLEAN=1 ./build.sh"
                echo ""
                echo "This will remove the build directory and ensure your changes are compiled."
                cd - >/dev/null
                rm -rf "$TEMP_DIR"
                exit 1
            else
                echo "✅ Build successful - new Hyprland binary generated"
                echo "   Previous binary (from $PREVIOUS_DEB_FILE): ${PRE_BUILD_BINARY_MD5:0:16}..."
                echo "   New binary (from $EXPECTED_DEB): ${POST_BUILD_BINARY_MD5:0:16}..."
                echo "   ✓ Binaries are different - code changes were compiled successfully"
            fi
        else
            echo "Post-build: Warning - could not extract Hyprland binary from new deb"
        fi
        cd - >/dev/null
        rm -rf "$TEMP_DIR"
        trap - EXIT
    elif [ -f "$EXPECTED_DEB" ]; then
        echo "✅ First build completed successfully - no previous version to compare against"
        echo "   Generated: $EXPECTED_DEB"
    fi

    # Show version info
    for deb in hyprmoon_*.deb; do
        if [ -f "$deb" ]; then
            VERSION=$(dpkg-deb --show --showformat='${Version}' "$deb")
            echo "Package: $deb"
            echo "Version: $VERSION"
        fi
    done

    echo ""
    echo "Build completed successfully!"
    echo "Build log file: $(find . -name "container-build-*.log" | sort | tail -1)"
    echo ""
    echo "Next steps:"
    echo "  1. Copy deb to helix: cp hyprmoon_*.deb /home/luke/pm/helix/"
    echo "  2. Update helix Dockerfile if needed"
    echo "  3. Rebuild helix container: cd /home/luke/pm/helix && docker compose -f docker-compose.dev.yaml build zed-runner"
    echo "  4. Restart helix: docker compose -f docker-compose.dev.yaml up -d zed-runner"
else
    echo "=== Build FAILED ==="
    echo "Docker build exited with code: $DOCKER_EXIT_CODE"
    echo "Check the log: $(find . -name "container-build-*.log" | sort | tail -1)"
    exit $DOCKER_EXIT_CODE
fi