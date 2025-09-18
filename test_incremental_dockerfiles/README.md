# Incremental Build Dockerfile Test Suite

This test suite validates the sed-based Dockerfile marker system that enables incremental binary deployment.

## Test Scenarios

### 1. **STANDARD → INCREMENTAL**
Initial: All incremental sections commented out
Result: Binary COPY uncommented with correct filename, version COPY uncommented, RUN block uncommented

### 2. **INCREMENTAL → STANDARD**
Initial: Incremental sections active
Result: All incremental sections commented out, template restored

### 3. **INCREMENTAL → INCREMENTAL** (Version Update)
Initial: Previous incremental version active
Result: Binary filename updated to new version, everything else stays uncommented

### 4. **STANDARD → STANDARD** (Version Update)
Initial: Incremental sections commented
Result: Remains commented, only .deb version numbers updated

## Marker System

**Dockerfile Template:**
```dockerfile
# COPY_BINARY_MARKER
# COPY Hyprland-VERSION /tmp/Hyprland-incremental
# COPY_VERSION_MARKER
# COPY HYPRMOON_VERSION.txt /tmp/
# RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \
#     && cp /tmp/Hyprland-incremental /usr/bin/Hyprland \
#     && chmod +x /usr/bin/Hyprland \
#     && cp /tmp/HYPRMOON_VERSION.txt /HYPRMOON_VERSION.txt \
#     && echo "✓ Binary clobbered with version: $(cat /HYPRMOON_VERSION.txt)" \
#     && rm -f /tmp/Hyprland-incremental /tmp/HYPRMOON_VERSION.txt
# INCREMENTAL_BINARY_COPY_END
```

**Markers:**
- `# COPY_BINARY_MARKER`: Targets next line for binary filename updates
- `# COPY_VERSION_MARKER`: Targets next line for comment/uncomment
- `# INCREMENTAL_BINARY_COPY_END`: Marks end of range for RUN block operations

## Running Tests

```bash
cd test_incremental_dockerfiles
./run_tests.sh
```

Tests validate all sed operations produce expected Dockerfile states for every build mode transition.