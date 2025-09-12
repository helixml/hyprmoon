// Wolf moonlight server implementation
// This provides the actual Wolf functionality integrated into Hyprland

#include "../wolf-impl/WolfMoonlightServer.hpp"

// Alias Wolf classes to the expected namespace
namespace wolf {
    using MoonlightState = core::MoonlightState;
    using StreamingEngine = core::StreamingEngine; 
    using ControlServer = core::ControlServer;
    using RestServer = core::RestServer;
} // namespace wolf