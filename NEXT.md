# NEXT.md - Incremental Screencopy Backend Plan

## Goal: Transform HyprMoon to use Screencopy with Stock Hyprland

Instead of creating a separate external project, we'll incrementally add a screencopy backend to the current working hyprmoon, then disable/remove the integrated Hyprland. This maintains functionality throughout the process.

## Current Status
- ✅ **Complete Wolf implementation working** (master branch)
- ✅ **Full 4-phase pairing protocol implemented**
- ✅ **Frame capture proof working** (grim capturing frames every 30 seconds)
- ✅ **All streaming infrastructure in place** (HTTP, HTTPS, RTSP, RTP, Control servers)
- ✅ **Native Hyprland frame capture working** (zero-copy DMA-BUF integration)
- ✅ **Wolf moonlight server fully functional** (WolfMoonlightServer, MoonlightManager, StreamingEngine)
- 🔄 **Ready for screencopy backend addition**

## Incremental Transformation Plan

### Phase 1: Add Screencopy Backend (Keep Hyprland Running)
1. **Add wlr-screencopy client** to HyprMoon project
2. **Implement screencopy frame source** alongside existing renderer capture
3. **Add config option** to choose between renderer capture vs screencopy
4. **Test dual mode** - HyprMoon can capture from both its own renderer AND external screencopy
5. **Verify streaming works** with screencopy frames

### Phase 2: Validate Screencopy-Only Mode (Hyprland Still Integrated)
1. **Add screencopy-only mode** config option
2. **Disable internal renderer capture** when in screencopy mode
3. **Test with stock Hyprland** running on different display
4. **Verify complete workflow** - pairing, streaming, input all work via screencopy
5. **Performance comparison** between modes

### Phase 3: Make Hyprland Optional (Preparation for Separation)
1. **Make Hyprland compositor optional** at runtime
2. **Add external Wayland session support**
3. **Test pure screencopy mode** with stock Hyprland as compositor
4. **Ensure all Wolf components work** without integrated Hyprland
5. **Validate input forwarding** to external compositor

### Phase 4: Extract Pure Screencopy Binary (Final Separation)
1. **Create build flag** to exclude Hyprland completely
2. **Extract minimal screencopy-only binary**
3. **Remove Hyprland dependencies** from screencopy build
4. **Package separately** as pure screencopy server
5. **Deploy side-by-side** with stock Hyprland

## Key Advantages of This Approach

### ✅ **Continuous Functionality**
- Never break the working system during transition
- Always have a working fallback mode
- Can compare performance at each step

### ✅ **Proven Infrastructure**
- Keep all working Wolf components intact
- Leverage complete 4-phase pairing protocol
- Maintain certificate management and crypto
- Preserve all streaming optimizations

### ✅ **Incremental Validation**
- Test each change independently
- Catch regressions immediately
- Validate assumptions step-by-step
- Easy rollback if issues found

### ✅ **Final Deployment Flexibility**
- Can run integrated HyprMoon for development
- Can run pure screencopy with stock Hyprland for production
- Can support both modes for different use cases

## Technical Implementation Details

### Screencopy Integration Points
1. **Frame Source Abstraction** - Create common interface for renderer vs screencopy frames
2. **Wolf Pipeline Compatibility** - Ensure screencopy frames work with existing GStreamer pipeline
3. **Config Management** - Extend existing Wolf config to support screencopy options
4. **Input Routing** - Verify input forwarding works with external compositor

### Build System Changes
1. **Conditional Compilation** - Use CMake flags to control Hyprland inclusion
2. **Dependency Management** - Make Hyprland dependencies optional
3. **Binary Variants** - Support building both integrated and screencopy-only versions
4. **Package Separation** - Create separate packages for different deployment modes

## Success Criteria
- [ ] Screencopy backend captures frames correctly
- [ ] Wolf streaming pipeline accepts screencopy frames
- [ ] Complete 4-phase pairing works via screencopy
- [ ] Input forwarding works with external compositor
- [ ] Performance comparable to integrated mode
- [ ] Can build and deploy screencopy-only binary
- [ ] Stock Hyprland + screencopy deployment working

## Why This Approach Will Succeed
1. **No Big-Bang Changes** - Incremental transformation reduces risk
2. **Leverage Proven Code** - Keep all working Wolf infrastructure
3. **Continuous Testing** - Catch issues early in the process
4. **Flexible Deployment** - Support multiple use cases
5. **Proven Foundation** - Start from working 4-phase pairing implementation