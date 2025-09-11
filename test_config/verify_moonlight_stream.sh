#!/bin/bash
set -e

echo "🔍 Verifying moonlight integration is actually working..."

# Wait for services to be available
echo "⏳ Waiting for moonlight services to start..."
timeout=30
count=0

while [ $count -lt $timeout ]; do
    if curl -s http://localhost:47989/serverinfo > /dev/null 2>&1; then
        echo "✅ HTTP server responding"
        break
    fi
    sleep 1
    count=$((count + 1))
done

if [ $count -ge $timeout ]; then
    echo "❌ Moonlight HTTP server never started"
    exit 1
fi

# Test the serverinfo endpoint
echo "🌐 Testing serverinfo endpoint..."
SERVER_INFO=$(curl -s http://localhost:47989/serverinfo)
echo "Server info response: $SERVER_INFO"

# Check if we can access RTSP
echo "📹 Testing RTSP stream availability..."
timeout 10 ffmpeg -f rtsp -i rtsp://localhost:48010/stream -t 5 -f mp4 -y /test_output/moonlight_stream.mp4 || echo "RTSP stream test completed (expected timeout)"

# Verify we got some video data
if [ -f /test_output/moonlight_stream.mp4 ]; then
    SIZE=$(stat -c%s /test_output/moonlight_stream.mp4)
    echo "📊 Captured moonlight stream: $SIZE bytes"
    if [ $SIZE -gt 1000 ]; then
        echo "✅ Successfully captured moonlight video stream!"
        # Extract a frame for verification
        ffmpeg -i /test_output/moonlight_stream.mp4 -vframes 1 -y /test_output/moonlight_frame.png 2>/dev/null || true
        if [ -f /test_output/moonlight_frame.png ]; then
            echo "🖼️  Extracted frame: $(stat -c%s /test_output/moonlight_frame.png) bytes"
        fi
        exit 0
    else
        echo "⚠️  Video file too small: $SIZE bytes"
    fi
else
    echo "❌ No video file captured"
fi

echo "🔍 Checking what processes are running..."
ps aux | grep -E "(Hyprland|moonlight)" || true

echo "🌐 Checking network services..."
netstat -tulpn | grep -E ":(4798[49]|48010|47999|48000|48002)" || true

exit 1