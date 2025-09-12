#!/bin/bash
set -e

echo "🔍 Deep Aquamarine backend debugging - testing all configurations"

# Setup environment
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p $XDG_RUNTIME_DIR
chmod 700 $XDG_RUNTIME_DIR

echo "📱 Testing different backend configurations..."

# Test 1: Force headless backend only
echo "🧪 Test 1: Headless backend only"
export WLR_BACKENDS=headless
export WLR_HEADLESS_OUTPUTS=1
timeout 10 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | head -20 || echo "Test 1 completed"

echo ""

# Test 2: Software rendering
echo "🧪 Test 2: Software rendering"
export WLR_RENDERER=pixman
export LIBGL_ALWAYS_SOFTWARE=1
timeout 10 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | head -20 || echo "Test 2 completed"

echo ""

# Test 3: Try without any GPU devices
echo "🧪 Test 3: No GPU devices"
unset WLR_DRM_DEVICES
export WLR_BACKENDS=headless
export WLR_RENDERER=pixman
timeout 10 /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | head -20 || echo "Test 3 completed"

echo ""

# Test 4: Check if binary has the right libraries
echo "🔗 Test 4: Binary library dependencies"
ldd /usr/bin/Hyprland | grep -E "(aquamarine|nvidia|drm|gbm)" || echo "No relevant libraries found"

echo ""

# Test 5: Run with verbose debug
echo "🧪 Test 5: Verbose debug output"
export HYPRLAND_LOG_WLR=1
export WAYLAND_DEBUG=1
export WLR_BACKENDS=headless
export WLR_RENDERER=pixman
timeout 10 strace -e openat /usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid 2>&1 | grep -E "(aquamarine|backend|create|failed)" | head -10 || echo "Test 5 completed"

echo "🏁 Deep debugging analysis complete"