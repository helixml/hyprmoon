#!/bin/bash
set -e

# Test suite for incremental build Dockerfile sed operations
echo "🧪 Dockerfile Marker System Test Suite"
echo "======================================"

# Create test workspace
mkdir -p test_workspace
cd test_workspace

# Test counter
TEST_NUM=0
PASS_COUNT=0
FAIL_COUNT=0

# Test helper functions
run_test() {
    local test_name="$1"
    local description="$2"
    TEST_NUM=$((TEST_NUM + 1))
    echo ""
    echo "Test $TEST_NUM: $test_name"
    echo "Description: $description"
    echo "----------------------------------------"
}

pass_test() {
    echo "✅ PASS: $1"
    PASS_COUNT=$((PASS_COUNT + 1))
}

fail_test() {
    echo "❌ FAIL: $1"
    FAIL_COUNT=$((FAIL_COUNT + 1))
}

check_line() {
    local expected="$1"
    local actual="$2"
    local description="$3"

    if [ "$actual" = "$expected" ]; then
        pass_test "$description"
    else
        fail_test "$description"
        echo "   Expected: $expected"
        echo "   Actual:   $actual"
    fi
}

# Extract lines for testing
get_binary_line() {
    grep -A1 "^# COPY_BINARY_MARKER" test_dockerfile.txt | tail -1
}

get_version_line() {
    grep -A1 "^# COPY_VERSION_MARKER" test_dockerfile.txt | tail -1
}

get_run_line() {
    grep "RUN echo.*INCREMENTAL DEPLOY" test_dockerfile.txt
}

# Simulate build.sh sed operations for INCREMENTAL mode
enable_incremental() {
    local latest_binary="$1"
    # Recreate the exact sed commands from build.sh
    sed -i '/^# COPY_BINARY_MARKER$/{ n; s/^# *COPY .*/COPY '"$latest_binary"' \/tmp\/Hyprland-incremental/; }' test_dockerfile.txt
    sed -i '/^# COPY_VERSION_MARKER$/{ n; s/^# *COPY/COPY/; }' test_dockerfile.txt
    sed -i '/^# RUN echo.*INCREMENTAL DEPLOY/,/^# INCREMENTAL_BINARY_COPY_END/s/^# //' test_dockerfile.txt
}

# Simulate build.sh sed operations for STANDARD mode
disable_incremental() {
    # Recreate the exact sed commands from build.sh
    sed -i '/^# COPY_BINARY_MARKER$/{ n; s/^COPY .*/# COPY Hyprland-VERSION \/tmp\/Hyprland-incremental/; }' test_dockerfile.txt
    sed -i '/^# COPY_VERSION_MARKER$/{ n; s/^COPY/# COPY/; }' test_dockerfile.txt
    sed -i '/^RUN echo.*INCREMENTAL DEPLOY/,/^INCREMENTAL_BINARY_COPY_END/s/^/# /' test_dockerfile.txt
}

# TEST 1: STANDARD → INCREMENTAL
run_test "STANDARD → INCREMENTAL" "Enable incremental deployment from standard template"
cp ../test_dockerfile_template.txt test_dockerfile.txt
enable_incremental "Hyprland-0.41.2+ds-1.3+step8.9.42"

check_line "COPY Hyprland-0.41.2+ds-1.3+step8.9.42 /tmp/Hyprland-incremental" "$(get_binary_line)" "Binary COPY uncommented with correct filename"
check_line "COPY HYPRMOON_VERSION.txt /tmp/" "$(get_version_line)" "Version COPY uncommented"
check_line "RUN echo \"🔥 INCREMENTAL DEPLOY: Clobbering with latest binary\" \\" "$(get_run_line)" "RUN block uncommented"

# TEST 2: INCREMENTAL → STANDARD
run_test "INCREMENTAL → STANDARD" "Disable incremental deployment back to standard"
disable_incremental

check_line "# COPY Hyprland-VERSION /tmp/Hyprland-incremental" "$(get_binary_line)" "Binary COPY commented out with template"
check_line "# COPY HYPRMOON_VERSION.txt /tmp/" "$(get_version_line)" "Version COPY commented out"
check_line "# RUN echo \"🔥 INCREMENTAL DEPLOY: Clobbering with latest binary\" \\" "$(get_run_line)" "RUN block commented out"

# TEST 3: INCREMENTAL → INCREMENTAL (Version Update)
run_test "INCREMENTAL → INCREMENTAL" "Update binary version while keeping incremental mode"
enable_incremental "Hyprland-0.41.2+ds-1.3+step8.9.43"
enable_incremental "Hyprland-0.41.2+ds-1.3+step8.9.44"

check_line "COPY Hyprland-0.41.2+ds-1.3+step8.9.44 /tmp/Hyprland-incremental" "$(get_binary_line)" "Binary filename updated to new version"
check_line "COPY HYPRMOON_VERSION.txt /tmp/" "$(get_version_line)" "Version COPY remains uncommented"
check_line "RUN echo \"🔥 INCREMENTAL DEPLOY: Clobbering with latest binary\" \\" "$(get_run_line)" "RUN block remains uncommented"

# TEST 4: STANDARD → STANDARD (Version Update)
run_test "STANDARD → STANDARD" "Keep standard mode across version updates"
disable_incremental
disable_incremental

check_line "# COPY Hyprland-VERSION /tmp/Hyprland-incremental" "$(get_binary_line)" "Binary COPY remains commented with template"
check_line "# COPY HYPRMOON_VERSION.txt /tmp/" "$(get_version_line)" "Version COPY remains commented"
check_line "# RUN echo \"🔥 INCREMENTAL DEPLOY: Clobbering with latest binary\" \\" "$(get_run_line)" "RUN block remains commented"

# TEST 5: INCREMENTAL → INCREMENTAL (Idempotent)
run_test "INCREMENTAL Idempotent" "Multiple incremental enables should be safe"
enable_incremental "Hyprland-0.41.2+ds-1.3+step8.9.45"
enable_incremental "Hyprland-0.41.2+ds-1.3+step8.9.45"
enable_incremental "Hyprland-0.41.2+ds-1.3+step8.9.45"

check_line "COPY Hyprland-0.41.2+ds-1.3+step8.9.45 /tmp/Hyprland-incremental" "$(get_binary_line)" "Idempotent: Binary filename correct after multiple enables"
check_line "COPY HYPRMOON_VERSION.txt /tmp/" "$(get_version_line)" "Idempotent: Version COPY correct after multiple enables"
check_line "RUN echo \"🔥 INCREMENTAL DEPLOY: Clobbering with latest binary\" \\" "$(get_run_line)" "Idempotent: RUN block correct after multiple enables"

# TEST 6: STANDARD → STANDARD (Idempotent)
run_test "STANDARD Idempotent" "Multiple standard disables should be safe"
disable_incremental
disable_incremental
disable_incremental

check_line "# COPY Hyprland-VERSION /tmp/Hyprland-incremental" "$(get_binary_line)" "Idempotent: Binary template correct after multiple disables"
check_line "# COPY HYPRMOON_VERSION.txt /tmp/" "$(get_version_line)" "Idempotent: Version COPY commented after multiple disables"
check_line "# RUN echo \"🔥 INCREMENTAL DEPLOY: Clobbering with latest binary\" \\" "$(get_run_line)" "Idempotent: RUN block commented after multiple disables"

# Final results
echo ""
echo "🏁 Test Results Summary"
echo "======================"
echo "✅ Passed: $PASS_COUNT"
echo "❌ Failed: $FAIL_COUNT"
echo "📊 Total:  $TEST_NUM tests"

if [ $FAIL_COUNT -eq 0 ]; then
    echo ""
    echo "🎉 ALL TESTS PASSED! The marker system is robust and handles all build mode transitions correctly."
    exit 0
else
    echo ""
    echo "💥 Some tests failed. Check the sed logic in build.sh for issues."
    exit 1
fi