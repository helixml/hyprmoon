#!/bin/bash
set -e

# Host-side trigger script for HyprMoon builds
# Usage: ./build.sh

echo "=== HyprMoon Build Trigger ==="
echo "Starting build at $(date)"

# Navigate to hyprmoon directory
cd /home/luke/pm/hyprmoon

# Run the container with the bind-mounted build script
echo "Executing container build..."
echo "Build logs will be saved to: container-build-*.log"
docker run --rm \
    -v $(pwd):/workspace \
    hyprmoon-build-env \
    /workspace/container-build.sh

# Check if build succeeded
if ls hyprmoon_*.deb >/dev/null 2>&1; then
    echo "=== Build SUCCESS ==="
    echo "Generated deb file(s):"
    ls -la hyprmoon_*.deb

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
    echo "No deb file generated. Check the log: $(find . -name "container-build-*.log" | sort | tail -1)"
    exit 1
fi