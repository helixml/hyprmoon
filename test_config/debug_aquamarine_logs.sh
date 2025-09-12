#!/bin/bash
set -e

echo "🔍 Capturing detailed Aquamarine failure logs"

# Setup environment
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p $XDG_RUNTIME_DIR
chmod 700 $XDG_RUNTIME_DIR

# Install strace for system call debugging
pacman -Sy --noconfirm strace

echo "📋 Test 1: Basic Hyprland startup with full error output"
export WLR_BACKENDS=headless
export WLR_RENDERER=pixman
export LIBGL_ALWAYS_SOFTWARE=1
export HYPRLAND_LOG_WLR=1

echo "--- Full Hyprland Error Output ---"
timeout 15 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 || true

echo ""
echo "📋 Test 2: System call trace during backend creation"
echo "--- System Call Trace ---"
timeout 10 strace -f -e trace=openat,access,mmap,ioctl /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | head -50 || true

echo ""
echo "📋 Test 3: Check what files/devices Hyprland tries to access"
echo "--- File Access Attempts ---"
timeout 10 strace -e trace=file /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | grep -E "(drm|gpu|render|card|aquamarine)" || true

echo ""
echo "📋 Test 4: Look for specific error patterns"
echo "--- Error Pattern Analysis ---"
timeout 10 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | grep -E -A5 -B5 "(failed|error|cannot|unable|FATAL|ERROR)" || true

echo ""
echo "📋 Test 5: Check Aquamarine library info"
echo "--- Aquamarine Library Details ---"
ldd /usr/lib64/libaquamarine.so.8 | head -10

echo ""
echo "📋 Test 6: Environment and dependency check"
echo "--- Environment State ---"
echo "EGL_PLATFORM: $EGL_PLATFORM"
echo "WLR_BACKENDS: $WLR_BACKENDS"
echo "WLR_RENDERER: $WLR_RENDERER"
ls -la /dev/dri/ || echo "No /dev/dri"
ls -la /sys/class/drm/ || echo "No /sys/class/drm"

echo "🏁 Detailed Aquamarine failure analysis complete"