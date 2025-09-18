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
echo ""

# Nice formatted table with performance analysis
tail -n +2 "$CSV_FILE" | tail -$NUM_ROWS | awk -F, '
BEGIN {
    print "Build | Type | Version | Duration | Improvement | ccache | Ninja | Binary MD5 | Git | Files | Status"
    print "------|------|---------|----------|-------------|--------|-------|------------|-----|-------|-------"
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

    printf "%2d. %-11s | %-7s | %8s | %-11s | %5s%% | %5s | %-10s | %-7s | %5s | %s\n",
        NR, type, version, $3 "s", improvement, $4, $12, $14, $15, $16, status

    prev_duration = $3
}' | column -t -s'|'

echo ""
echo "📊 Build Type Summary:"
echo "CLEAN: Full dpkg-buildpackage with all dependencies"
echo "INCREMENTAL: Direct ninja compilation with binary clobbering"
echo ""
echo "⚡ LIGHTNING: <10s (no changes) | 🚀 FAST: <100s | 🏃 GOOD: <300s | 🐌 SLOW: >300s"
# Check for running build container
if docker ps --format "table {{.Names}}\t{{.Status}}" | grep -q "hyprmoon-builder"; then
    CONTAINER_STATUS=$(docker ps --format "table {{.Names}}\t{{.Status}}" | grep "hyprmoon-builder" | awk '{print $2, $3}')
    echo "🔄 BUILD CURRENTLY RUNNING:"
    echo "Container: hyprmoon-builder | Status: $CONTAINER_STATUS"
    echo ""
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