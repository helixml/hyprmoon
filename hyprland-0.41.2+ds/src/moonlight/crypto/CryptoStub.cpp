#include "CryptoStub.hpp"
#include "../../debug/Log.hpp"

namespace moonlight::crypto {

void CryptoStub::init() {
    Debug::log(LOG, "[moonlight] Crypto subsystem initialized (stub)");
}

void CryptoStub::destroy() {
    Debug::log(LOG, "[moonlight] Crypto subsystem destroyed");
}

} // namespace moonlight::crypto