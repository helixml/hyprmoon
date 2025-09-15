#!/bin/bash

# Simple script to tail the latest build log file
# Usage: ./tail-latest-build.sh [number of lines]

# Default to 20 lines if no argument provided
LINES=${1:-20}

# Find the latest build log file
LATEST_LOG=$(ls -t build-*.log 2>/dev/null | head -1)

if [ -z "$LATEST_LOG" ]; then
    echo "No build log files found (build-*.log)"
    exit 1
fi

echo "Tailing latest build log: $LATEST_LOG"
echo "----------------------------------------"

# Tail the file with specified number of lines
tail -f -n "$LINES" "$LATEST_LOG"