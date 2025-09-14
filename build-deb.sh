#!/bin/bash
set -e

echo "Building HyprMoon with Ubuntu caching..."

# Build the build container first
echo "Building build container with dependencies..."
docker build -f Dockerfile.build -t hyprmoon-build-env . || exit 1

# Use BuildKit mount caches for maximum build speed
echo "Building HyprMoon deb package with cached build environment..."
docker run --rm \
    --mount=type=bind,source="$(pwd)",target=/workspace \
    --mount=type=cache,target=/ccache \
    --mount=type=cache,target=/root/.cache/meson \
    hyprmoon-build-env \
    bash -c "
        cd /workspace/hyprland-0.41.2+ds

        # Ensure build dependencies are satisfied
        echo 'Installing any missing build dependencies...'
        apt-get update
        mk-build-deps -i -r -t 'apt-get -y' debian/control

        # Clean any previous builds
        echo 'Cleaning previous builds...'
        fakeroot debian/rules clean || true

        # Configure ccache
        echo 'Configuring ccache...'
        ccache --set-config=max_size=2G
        ccache --zero-stats

        # Build with full optimization and caching
        echo 'Building HyprMoon deb package...'
        export DEB_BUILD_OPTIONS='parallel=\$(nproc)'
        export CCACHE_DIR=/ccache
        dpkg-buildpackage -us -uc -b

        # Show ccache stats
        echo 'ccache statistics:'
        ccache --show-stats

        # List built packages
        echo 'Built packages:'
        ls -la ../*.deb
    "

echo "Build complete! Packages are in the parent directory."