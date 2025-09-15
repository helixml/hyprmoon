#include "RTSPStub.hpp"
#include "../../debug/Log.hpp"

namespace moonlight::protocol {

void RTSPStub::init() {
    Debug::log(LOG, "[moonlight] RTSP protocol initialized (stub)");
}

void RTSPStub::destroy() {
    Debug::log(LOG, "[moonlight] RTSP protocol destroyed");
}

} // namespace moonlight::protocol