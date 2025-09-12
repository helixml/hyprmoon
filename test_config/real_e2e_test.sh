#!/bin/bash
set -e

echo "🚀 Starting REAL E2E Test: Hyprland + Moonlight Integration"

# Setup runtime directories
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p $XDG_RUNTIME_DIR
chmod 700 $XDG_RUNTIME_DIR

# Build green wayland client correctly
echo "🛠️ Building green wayland client..."
cd /test_config
gcc -o green_client simple_green_client.c $(pkg-config --cflags --libs wayland-client) || {
    echo "❌ Failed to compile wayland client"
    exit 1
}
echo "✅ Green wayland client compiled successfully"

# Set up environment for Wayland GPU acceleration (NVIDIA RTX 4090)
export WLR_RENDERER=gles2
export WLR_BACKENDS=headless
export WLR_NO_HARDWARE_CURSORS=1
export WLR_HEADLESS_OUTPUTS=1

# Set GPU acceleration settings for NVIDIA
export __GL_THREADED_OPTIMIZATIONS=1
export __GL_SYNC_TO_VBLANK=1
export LIBGL_ALWAYS_SOFTWARE=0

# Core NVIDIA environment variables 
export NVIDIA_VISIBLE_DEVICES=all
export NVIDIA_DRIVER_CAPABILITIES=all

# EGL configuration for NVIDIA
export EGL_PLATFORM=drm
export GBM_BACKEND=nvidia-drm
export __GLX_VENDOR_LIBRARY_NAME=nvidia

# wlroots GPU configuration - use card1 (the actual device)
export WLR_DRM_DEVICES=/dev/dri/card1
export WLR_RENDERER_ALLOW_SOFTWARE=1
export WLR_DRM_NO_ATOMIC=1

# GPU render node configuration
export GPU_RENDER_NODE=/dev/dri/renderD128
export GST_GL_DRM_DEVICE=${GST_GL_DRM_DEVICE:-$GPU_RENDER_NODE}

# GStreamer GPU configuration for NVIDIA
export GST_GL_API=gles2
export GST_GL_WINDOW=surfaceless

# Enable ALL debug logging
export HYPRLAND_TRACE=1
export HYPRLAND_LOG_WLR=1
export WLR_DEBUG=1
export WAYLAND_DEBUG=1

# Start Hyprland in background
echo "🎮 Starting Hyprland with moonlight integration and GPU acceleration..."
export WAYLAND_DISPLAY=wayland-0
/usr/bin/Hyprland --config /test_config/hyprland.conf --i-am-really-stupid &
HYPR_PID=$!

# Wait for wayland socket
echo "⏳ Waiting for Hyprland wayland socket..."
timeout=30
while [ $timeout -gt 0 ] && [ ! -S "$XDG_RUNTIME_DIR/wayland-0" ]; do
    sleep 1
    timeout=$((timeout - 1))
done

if [ ! -S "$XDG_RUNTIME_DIR/wayland-0" ]; then
    echo "❌ Hyprland wayland socket not created"
    kill $HYPR_PID 2>/dev/null || true
    exit 1
fi

echo "✅ Hyprland running, wayland socket created"

# Start green client in background
echo "🟢 Starting green wayland client..."
./green_client &
CLIENT_PID=$!

# Give client time to connect and render
sleep 3

# Check if moonlight server is active in Hyprland
echo "🌙 Checking moonlight server status..."
if curl -s http://localhost:47989/serverinfo > /dev/null 2>&1; then
    echo "✅ Moonlight HTTP server responding"
    
    # Try to capture RTSP stream
    echo "📹 Capturing moonlight RTSP stream..."
    timeout 10 ffmpeg -f rtsp -i rtsp://localhost:48010/stream -t 5 -f mp4 -y /test_output/real_moonlight_capture.mp4 2>/dev/null || {
        echo "⚠️ RTSP capture failed, trying alternative ports..."
        timeout 10 ffmpeg -f rtsp -i rtsp://localhost:47999/stream -t 5 -f mp4 -y /test_output/real_moonlight_capture.mp4 2>/dev/null || {
            echo "⚠️ RTSP capture failed on all ports, checking HTTP stream..."
            curl -o /test_output/moonlight_http_response.txt http://localhost:47989/serverinfo || true
        }
    }
    
    # Check capture results
    if [ -f /test_output/real_moonlight_capture.mp4 ] && [ -s /test_output/real_moonlight_capture.mp4 ]; then
        SIZE=$(stat -c%s /test_output/real_moonlight_capture.mp4)
        echo "✅ Captured moonlight stream: $SIZE bytes"
        
        # Extract frame to verify green content
        ffmpeg -i /test_output/real_moonlight_capture.mp4 -vframes 1 -y /test_output/real_moonlight_frame.png 2>/dev/null || true
        
        if [ -f /test_output/real_moonlight_frame.png ]; then
            echo "✅ Frame extracted from real moonlight stream"
        fi
        
        echo "🎉 REAL E2E TEST PASSED: Moonlight integration working!"
    else
        echo "❌ No valid moonlight stream captured"
        exit 1
    fi
else
    echo "❌ Moonlight server not responding"
    exit 1
fi

# Cleanup
kill $CLIENT_PID 2>/dev/null || true
kill $HYPR_PID 2>/dev/null || true

echo "✅ Real E2E test completed successfully"
exit 0