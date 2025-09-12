#!/bin/bash
# Test if our meson configuration works with system dependencies
echo "Testing meson configuration with system dependencies..."

docker run --rm -v $(pwd):/workspace ubuntu:25.04 bash -c "
apt-get update >/dev/null 2>&1
apt-get install -y meson ninja-build pkgconf jq \
  libwlroots-0.18-dev libudis86-dev \
  libhyprlang-dev libhyprcursor-dev libhyprutils-dev \
  hyprland-protocols hyprwayland-scanner \
  libcairo-dev libdrm-dev libxkbcommon-dev \
  libegl-dev libinput-dev libpango1.0-dev \
  libpixman-1-dev libseat-dev libtomlplusplus-dev \
  libudev-dev libwayland-dev libxcb-errors-dev \
  libxcb-util-dev wayland-protocols \
  libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
  libgstreamer-plugins-bad1.0-dev \
  libenet-dev libssl-dev libfmt-dev \
  libboost-system-dev libboost-filesystem-dev \
  libboost-locale-dev libboost-thread-dev \
  libpci-dev libcurl4-openssl-dev >/dev/null 2>&1

cd /workspace
echo '🔧 Testing meson setup with system dependencies...'
meson setup build-test --prefix=/usr/local --buildtype=release \
  -Dwith_moonlight=true 2>&1 | tail -10

echo '📋 Meson configuration result:'
if [ -f build-test/meson-private/build.dat ]; then
  echo '✅ Meson configuration succeeded with system dependencies!'
  echo '🎉 This means our Ubuntu approach works!'
else
  echo '❌ Meson configuration failed'
fi
"