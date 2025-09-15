#!/bin/bash
set -e

echo "Building HyprMoon with Ubuntu caching..."

# Build the build container first
echo "Building build container with dependencies..."
docker build -f Dockerfile.build -t hyprmoon-build-env . || exit 1

# Use simple bind mount (Docker version doesn't support cache mounts)
echo "Building HyprMoon deb package with build environment..."
docker run --rm \
    -v "$(pwd):/workspace" \
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

        # Build with parallel compilation
        echo 'Building HyprMoon deb package...'
        export DEB_BUILD_OPTIONS='parallel=\$(nproc)'
        dpkg-buildpackage -us -uc -b

        # List built packages
        echo 'Built packages:'
        ls -la ../*.deb
    "

echo "Build complete! Packages are in the parent directory."