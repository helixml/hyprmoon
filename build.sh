#!/bin/bash
set -e

# Function to log exit with reason (for explicit use only)
exit_with_reason() {
    local exit_code=$1
    local reason="$2"
    echo ""
    echo "🚨 === SCRIPT EXITING ==="
    echo "Exit code: $exit_code"
    echo "Reason: $reason"
    echo "Time: $(date)"
    echo "=== END EXIT LOG ==="
    exit $exit_code
}

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
echo "About to run docker container..."
echo "Docker command: docker run --rm -v $(pwd):/workspace -v /home/luke/.ccache:/ccache hyprmoon-build-env /workspace/container-build.sh"

docker run --rm \
    -v $(pwd):/workspace \
    -v /home/luke/.ccache:/ccache \
    hyprmoon-build-env \
    /workspace/container-build.sh

echo ""
echo "🔍 === CRITICAL DEBUG: Docker command completed ==="
echo "Docker run command returned with exit code: $?"
echo "Host script continuing after docker run..."
echo "Current directory: $(pwd)"
echo "Script PID: $$"
echo "Bash version: $BASH_VERSION"
echo "Time: $(date)"
echo ""

# Capture docker exit code
DOCKER_EXIT_CODE=$?
echo "🔍 DOCKER_EXIT_CODE captured as: $DOCKER_EXIT_CODE"

echo ""
echo "=== POST-CONTAINER DEBUG INFO ==="
echo "Docker container exit code: $DOCKER_EXIT_CODE"
echo "Current working directory: $(pwd)"
echo "Available .deb files in current directory:"
ls -la *.deb 2>/dev/null || echo "  No .deb files found in current directory"
echo "Expected deb file: $EXPECTED_DEB"
echo "Pre-build binary MD5: $PRE_BUILD_BINARY_MD5"
echo "Checking if expected deb exists: $([ -f "$EXPECTED_DEB" ] && echo "YES" || echo "NO")"
echo "=== END DEBUG INFO ==="
echo ""

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
                # STILL DEPLOY even with stale cache - user might want to test existing build
                echo ""
                echo "⚠️ WARNING: Continuing with auto-deployment despite stale cache..."
                echo "   The deployed version will have the old binary, but deployment will proceed."
                echo "   Stale cache will not prevent auto-deployment."
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
    echo "Package details:"
    set +e
    for deb in hyprmoon_*.deb; do
        if [ -f "$deb" ]; then
            # Extract version from filename since dpkg-deb --show can be unreliable
            VERSION=$(echo "$deb" | sed 's/hyprmoon_\(.*\)_amd64.deb/\1/')
            SIZE=$(stat -c%s "$deb")
            echo "Package: $deb"
            echo "Version: $VERSION"
            echo "Size: $SIZE bytes"

            # Try to get package info, but don't fail if it errors
            if dpkg-deb --info "$deb" >/dev/null 2>&1; then
                echo "Package info: $(dpkg-deb --field "$deb" Package) $(dpkg-deb --field "$deb" Version)"
            else
                echo "Package info: Unable to read package metadata (non-fatal)"
            fi
        fi
    done
    set -e

    echo ""
    echo "Build completed successfully!"
    echo "Build log file: $(find . -name "container-build-*.log" | sort | tail -1)"
    echo ""

    # AUTO-DEPLOY: Copy packages to helix and update Docker
    echo "=== AUTO-DEPLOY: Deploying to Helix ==="
    echo "Current directory: $(pwd)"
    echo "Checking for helix directory..."

    # Step 1: Copy new deb files to helix
    HELIX_DIR="../helix"
    echo "Expected helix path: $HELIX_DIR"
    echo "Absolute helix path: $(realpath "$HELIX_DIR" 2>/dev/null || echo "Path resolution failed")"

    if [ ! -d "$HELIX_DIR" ]; then
        echo "❌ ERROR: Helix directory not found at $HELIX_DIR"
        echo "   Expected helix repo to be adjacent to hyprmoon repo"
        echo "   Current structure should be:"
        echo "     parent-dir/"
        echo "       ├── hyprmoon/"
        echo "       └── helix/"
        exit_with_reason 1 "Helix directory not found at $HELIX_DIR - cannot deploy"
    fi

    echo "1. Copying deb files to $HELIX_DIR/"
    cp hyprmoon_*.deb "$HELIX_DIR/"

    # Check if backgrounds package exists and is not corrupted before copying
    if ls hyprland-backgrounds_*.deb >/dev/null 2>&1; then
        for bg_deb in hyprland-backgrounds_*.deb; do
            if [ -f "$bg_deb" ]; then
                # Check if package is reasonable size (should be ~46MB, not <1KB)
                BG_SIZE=$(stat -c%s "$bg_deb")
                if [ "$BG_SIZE" -gt 1000000 ]; then
                    echo "   Copying backgrounds package: $bg_deb (size: $BG_SIZE bytes)"
                    cp "$bg_deb" "$HELIX_DIR/"
                else
                    echo "   ⚠️ Skipping corrupted backgrounds package: $bg_deb (size: $BG_SIZE bytes)"
                fi
            fi
        done
    else
        echo "   (No backgrounds package found - that's OK)"
    fi

    # Step 2: Update Dockerfile.zed-agent-vnc with new version numbers
    echo "2. Updating Dockerfile.zed-agent-vnc with new package versions"
    cd "$HELIX_DIR"

    # Get the new version numbers from the copied debs
    NEW_VERSION=$(ls hyprmoon_*.deb | head -1 | sed 's/hyprmoon_\(.*\)_amd64.deb/\1/')
    OLD_VERSION_PATTERN="step8\.9\.[0-9]\+"

    echo "   Updating from pattern $OLD_VERSION_PATTERN to $NEW_VERSION"

    # Update the COPY commands and dpkg install commands
    sed -i "s/hyprmoon_0\.41\.2+ds-1\.3+${OLD_VERSION_PATTERN}_amd64\.deb/hyprmoon_${NEW_VERSION}_amd64.deb/g" Dockerfile.zed-agent-vnc
    sed -i "s/hyprland-backgrounds_0\.41\.2+ds-1\.3+${OLD_VERSION_PATTERN}_all\.deb/hyprland-backgrounds_${NEW_VERSION}_all.deb/g" Dockerfile.zed-agent-vnc

    echo "   ✓ Dockerfile updated with version $NEW_VERSION"

    # Step 3: Check if zed-runner container is running and rebuild
    echo "3. Checking if zed-runner container is running..."
    if docker compose -f docker-compose.dev.yaml ps zed-runner | grep -q "Up"; then
        CONTAINER_WAS_RUNNING=true
        echo "   Container is running - will restart after rebuild"
    else
        CONTAINER_WAS_RUNNING=false
        echo "   Container is not running - will build only"
    fi

    # Step 4: Rebuild container
    echo "4. Rebuilding zed-runner container with new HyprMoon packages..."
    docker compose -f docker-compose.dev.yaml build zed-runner

    # Step 5: Restart if it was running
    if [ "$CONTAINER_WAS_RUNNING" = true ]; then
        echo "5. Restarting zed-runner container..."
        docker compose -f docker-compose.dev.yaml up -d zed-runner
        echo "   ✓ Container restarted with new HyprMoon version $NEW_VERSION"
    else
        echo "5. Container was not running - build complete, ready to start"
    fi

    echo ""
    echo "🚀 AUTO-DEPLOY COMPLETED SUCCESSFULLY!"
    echo "   HyprMoon version: $NEW_VERSION"
    echo "   Container status: $([ "$CONTAINER_WAS_RUNNING" = true ] && echo "RESTARTED" || echo "BUILT (not running)")"
    echo ""
    echo "Ready to test Moonlight pairing with certificate fixes!"

    cd - >/dev/null  # Return to hyprmoon directory

    echo ""
    echo "🎉 === BUILD.SH COMPLETED SUCCESSFULLY ==="
    echo "All tasks completed: build, deploy, and container restart"
    echo "Script finished normally at $(date)"
    exit 0
else
    echo "=== Build FAILED ==="
    echo "Docker build exited with code: $DOCKER_EXIT_CODE"
    echo "Check the log: $(find . -name "container-build-*.log" | sort | tail -1)"
    exit_with_reason $DOCKER_EXIT_CODE "Docker container build failed with exit code $DOCKER_EXIT_CODE"
fi
