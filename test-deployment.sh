#!/bin/bash
set -e

# Test script to verify auto-deployment logic
echo "=== TESTING AUTO-DEPLOYMENT LOGIC ==="

# Simulate successful Docker build
DOCKER_EXIT_CODE=0
EXPECTED_DEB="hyprmoon_0.41.2+ds-1.3+step8.9.10_amd64.deb"
PRE_BUILD_BINARY_MD5=""

cd /home/luke/pm/hyprmoon

if [ $DOCKER_EXIT_CODE -eq 0 ]; then
    echo "=== Build SUCCESS (simulated) ==="
    echo "Generated deb file(s):"
    ls -la hyprmoon_*.deb

    echo ""
    echo "Build completed successfully!"
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
        exit 1
    fi

    echo "1. Copying deb files to $HELIX_DIR/"
    cp hyprmoon_*.deb "$HELIX_DIR/"
    cp hyprland-backgrounds_*.deb "$HELIX_DIR/" 2>/dev/null || echo "   (No backgrounds package found - that's OK)"

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
    echo "🎉 === DEPLOYMENT TEST COMPLETED SUCCESSFULLY ==="
fi