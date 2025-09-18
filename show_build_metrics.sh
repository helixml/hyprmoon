#!/bin/bash
# HyprMoon Build Performance Metrics Display Script

CSV_FILE="${1:-build-metrics.csv}"
NUM_ROWS="${2:-10}"

if [ ! -f "$CSV_FILE" ]; then
    echo "❌ CSV file not found: $CSV_FILE"
    echo "Usage: $0 [csv_file] [num_rows]"
    exit 1
fi

echo "🚀 HyprMoon Build Performance Metrics"
echo "====================================="
echo "File: $CSV_FILE"
echo "Showing last $NUM_ROWS builds"
echo "Generated: $(date '+%Y-%m-%d %H:%M:%S')"
echo ""

# Check for running build container first
RUNNING_BUILD_ROW=""
if docker ps --format "table {{.Names}}\t{{.Status}}" | grep -q "hyprmoon-builder"; then
    # Get container creation timestamp and calculate elapsed time
    CREATED_TIMESTAMP=$(docker inspect hyprmoon-builder --format='{{.Created}}' 2>/dev/null | sed 's/T/ /' | cut -d. -f1)
    START_EPOCH=$(date -d "$CREATED_TIMESTAMP" +%s 2>/dev/null || echo "0")
    CURRENT_EPOCH=$(date +%s)
    ELAPSED_SECONDS=$((CURRENT_EPOCH - START_EPOCH))

    # Format elapsed time nicely
    if [ $ELAPSED_SECONDS -lt 60 ]; then
        ELAPSED_TIME="${ELAPSED_SECONDS}s"
    else
        ELAPSED_MIN=$((ELAPSED_SECONDS / 60))
        ELAPSED_SEC=$((ELAPSED_SECONDS % 60))
        ELAPSED_TIME="${ELAPSED_MIN}m${ELAPSED_SEC}s"
    fi

    # Get current version being built
    CURRENT_VERSION=$(grep -m1 '^hyprmoon (' hyprland-0.41.2+ds/debian/changelog | sed 's/hyprmoon (\([^)]*\)).*/\1/' | awk -F. '{print $NF}')

    RUNNING_BUILD_ROW="🔄|$(date '+%H:%M:%S')|BUILDING|.$CURRENT_VERSION|$ELAPSED_TIME|RUNNING|TBD|TBD|TBD|TBD|TBD|🔄 LIVE"
fi

# Nice formatted table with performance analysis
tail -n +2 "$CSV_FILE" | tail -$NUM_ROWS | awk -F, '
BEGIN {
    prev_duration = 0
}
{
    # Determine build type
    type = ($17 == "FULL" ? "CLEAN" : "INCREMENTAL")

    # Calculate improvement
    improvement = ""
    if (NR > 1 && prev_duration > 0 && $3 > 0) {
        improvement = sprintf("%.1fx", prev_duration / $3)
    }

    # Performance status
    status = "🐌 SLOW"
    if ($3 < 10) status = "⚡ LIGHTNING"
    else if ($3 < 100) status = "🚀 FAST"
    else if ($3 < 300) status = "🏃 GOOD"

    # Format version (last part only)
    version = substr($2, length($2) - 4)

    # Format timestamp
    timestamp = strftime("%H:%M:%S", $1)

    printf "%2d|%8s|%11s|%7s|%8s|%11s|%6s|%5s|%10s|%8s|%5s|%s\n",
        NR, timestamp, type, version, $3 "s", improvement, $4 "%", $12, $14, $15, $16, status

    prev_duration = $3
}' | (echo "Build|Time|Type|Version|Duration|Improvement|ccache|Ninja|Binary MD5|Git|Files|Status"; echo "-----|----|----|----|----|-----------|------|-----|----------|---|-----|------"; cat; if [ -n "$RUNNING_BUILD_ROW" ]; then echo "$RUNNING_BUILD_ROW"; fi) | column -t -s'|'

echo ""
echo "📊 Build Type Summary:"
echo "CLEAN: Full dpkg-buildpackage with all dependencies"
echo "INCREMENTAL: Direct ninja compilation with binary clobbering"
echo ""
echo "⚡ LIGHTNING: <10s (no changes) | 🚀 FAST: <100s | 🏃 GOOD: <300s | 🐌 SLOW: >300s"
# Check for running build container and add live row
RUNNING_BUILD=""
if docker ps --format "table {{.Names}}\t{{.Status}}" | grep -q "hyprmoon-builder"; then
    # Get container creation timestamp and calculate elapsed time
    CREATED_TIMESTAMP=$(docker inspect hyprmoon-builder --format='{{.Created}}' 2>/dev/null | sed 's/T/ /' | cut -d. -f1)
    START_EPOCH=$(date -d "$CREATED_TIMESTAMP" +%s 2>/dev/null || echo "0")
    CURRENT_EPOCH=$(date +%s)
    ELAPSED_SECONDS=$((CURRENT_EPOCH - START_EPOCH))

    # Format elapsed time nicely
    if [ $ELAPSED_SECONDS -lt 60 ]; then
        ELAPSED_TIME="${ELAPSED_SECONDS}s"
    else
        ELAPSED_MIN=$((ELAPSED_SECONDS / 60))
        ELAPSED_SEC=$((ELAPSED_SECONDS % 60))
        ELAPSED_TIME="${ELAPSED_MIN}m${ELAPSED_SEC}s"
    fi

    # Get current version being built
    CURRENT_VERSION=$(grep -m1 '^hyprmoon (' hyprland-0.41.2+ds/debian/changelog | sed 's/hyprmoon (\([^)]*\)).*/\1/' | awk -F. '{print $NF}')

    RUNNING_BUILD=$(printf "%2s|%11s|%7s|%8s|%11s|%6s|%5s|%10s|%8s|%5s|%s\n" "🔄" "BUILDING" ".$CURRENT_VERSION" "$ELAPSED_TIME" "RUNNING" "TBD" "TBD" "TBD" "TBD" "TBD" "🔄 LIVE")
fi

# Check deployed version in helix container
if docker compose -f ../helix/docker-compose.dev.yaml ps zed-runner 2>/dev/null | grep -q "Up"; then
    CONTAINER_VERSION=$(docker compose -f ../helix/docker-compose.dev.yaml exec zed-runner cat /HYPRMOON_VERSION.txt 2>/dev/null || echo "unknown")
    CONTAINER_AGE=$(docker compose -f ../helix/docker-compose.dev.yaml ps zed-runner --format "table {{.Status}}" | tail -1 | sed 's/Up //' | sed 's/ (.*//')
    echo "🐳 DEPLOYED CONTAINER STATUS:"
    echo "Version: $CONTAINER_VERSION | Running for: $CONTAINER_AGE"
    echo ""
fi

echo "💡 Pro tips:"
echo "• For detailed ninja analysis: cat ninja-debug.log"
echo "• For live build monitoring: docker logs hyprmoon-builder -f"
echo "• For ccache stats: CCACHE_DIR=/home/luke/.ccache ccache -s"