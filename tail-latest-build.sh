#!/bin/bash

# Smart script to tail the latest build log file and auto-switch to newer ones
# Usage: ./tail-latest-build.sh [number of lines]

# Default to 20 lines if no argument provided
LINES=${1:-20}

# Function to find the latest build log file
find_latest_log() {
    ls -t container-build-*.log 2>/dev/null | head -1
}

# Function to get file modification time
get_file_mtime() {
    stat -c %Y "$1" 2>/dev/null
}

# Initial check
CURRENT_LOG=$(find_latest_log)

if [ -z "$CURRENT_LOG" ]; then
    echo "No build log files found (container-build-*.log)"
    echo "Waiting for new build files..."
    # Wait for first log file to appear
    while [ -z "$CURRENT_LOG" ]; do
        sleep 1
        CURRENT_LOG=$(find_latest_log)
    done
fi

echo "Watching for build logs, starting with: $CURRENT_LOG"
echo "----------------------------------------"

# Start tailing current file in background
tail -f -n "$LINES" "$CURRENT_LOG" &
TAIL_PID=$!

# Monitor for newer files
CURRENT_MTIME=$(get_file_mtime "$CURRENT_LOG")

while true; do
    sleep 2

    # Check for newer log file
    LATEST_LOG=$(find_latest_log)

    if [ "$LATEST_LOG" != "$CURRENT_LOG" ]; then
        LATEST_MTIME=$(get_file_mtime "$LATEST_LOG")

        if [ "$LATEST_MTIME" -gt "$CURRENT_MTIME" ]; then
            echo ""
            echo "🔄 Switching to newer build log: $LATEST_LOG"
            echo "----------------------------------------"

            # Kill old tail and start new one
            kill $TAIL_PID 2>/dev/null
            tail -f -n "$LINES" "$LATEST_LOG" &
            TAIL_PID=$!

            # Update tracking variables
            CURRENT_LOG="$LATEST_LOG"
            CURRENT_MTIME="$LATEST_MTIME"
        fi
    fi
done
