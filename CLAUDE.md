# Claude Rules for HyprMoon Development

This file contains critical development guidelines and context that MUST be followed at all times during HyprMoon development.

## CRITICAL: Always Reference design.md

Before taking any action, ALWAYS read and follow the current `/home/luke/pm/hyprmoon/design.md` file. This file contains:
- Current development context and directory structure
- Key development lessons learned
- Previous findings and known issues
- Current problem description
- Methodical approach phases and steps
- Success criteria

## Key Development Rules (from design.md)

### ALWAYS Follow These Rules:
1. **Always run builds in foreground**: Never use `&` or background mode for build commands, be patient
2. **Build caches are critical**: Without ccache/meson cache, iteration takes too long
3. **Test after every change**: Big-bang approaches are impossible to debug
4. **Use exact Ubuntu source**: Don't deviate from what Ubuntu ships
5. **Container build caches matter**: Use BuildKit mount caches for Docker builds
6. **Git commit discipline**: Make commits after every substantial change
7. **Phase milestone commits**: ALWAYS commit when reaching phase milestones
8. **Manual testing required**: Human verification at every step, no automation
9. **CRITICAL: Always start helix container before manual testing**: MUST check `docker ps | grep helix` and start container if needed before asking user to test via VNC
10. **MANDATORY 60-SECOND BUILD MONITORING**: ALWAYS monitor builds every 60 seconds using BashOutput tool until completion - NEVER start a build and forget about it
11. **NEVER GIVE UP ON LONG BUILDS**: ALWAYS wait patiently for builds to complete, no matter how long they take - builds can take 10+ minutes, be patient and keep monitoring every 60 seconds

### Current Development Context:
- **New methodical repo**: `~/pm/hyprmoon/` (this directory)
- **Previous attempt**: `~/pm/Hyprland-wlroots/` (big-bang approach, caused grey screen)
- **Helix container environment**: `/home/luke/pm/helix/` (test environment)
- **Ubuntu source**: `~/pm/hyprmoon/hyprland-0.41.2+ds/` (baseline)

### Current Problem:
- HyprMoon container shows grey screen in VNC instead of working helix desktop
- Need to isolate which modification broke the rendering/capture
- Using methodical incremental approach instead of big-bang

### Current Status:
Phase 1 Complete: Raw Ubuntu package built and ready for testing
Next: Test baseline, then incremental moonlight integration

## MUST ALWAYS DO BEFORE MANUAL TESTING:
1. Check if helix container is running: `docker ps | grep helix`
2. If not running, start it before asking user to test
3. Provide VNC connection details (port 5901)
4. Only then ask user to manually test via VNC

This file must be kept up to date with any critical lessons learned during development.