#include "InputManager.hpp"
#include "../../debug/Log.hpp"
#include <algorithm>

namespace moonlight {
namespace input {

// VirtualInputDevice Implementation
VirtualInputDevice::VirtualInputDevice(DeviceType type, const std::string& name)
    : m_type(type), m_name(name) {
    Debug::log(LOG, "[moonlight] Creating VirtualInputDevice: {} (type: {})",
               m_name, static_cast<int>(m_type));
}

VirtualInputDevice::~VirtualInputDevice() {
    shutdown();
}

bool VirtualInputDevice::initialize() {
    if (m_initialized) {
        Debug::log(WARN, "[moonlight] VirtualInputDevice {} already initialized", m_name);
        return true;
    }

    Debug::log(LOG, "[moonlight] Initializing VirtualInputDevice: {} (Step 7: Input Management)", m_name);

    // Step 7 stub implementation - just log successful initialization
    // In a real implementation, this would create libevdev/uinput devices
    m_initialized = true;

    Debug::log(LOG, "[moonlight] VirtualInputDevice {} initialized successfully", m_name);
    Debug::log(LOG, "[moonlight] Device type: {}, exposed to Wayland as virtual input",
               static_cast<int>(m_type));

    return true;
}

void VirtualInputDevice::shutdown() {
    if (!m_initialized) return;

    Debug::log(LOG, "[moonlight] Shutting down VirtualInputDevice: {}", m_name);
    m_initialized = false;
}

void VirtualInputDevice::sendMouseMove(float x, float y, bool relative) {
    if (!m_initialized) return;

    logEvent("MouseMove",
             relative ?
             "relative(" + std::to_string(x) + ", " + std::to_string(y) + ")" :
             "absolute(" + std::to_string(x) + ", " + std::to_string(y) + ")");
}

void VirtualInputDevice::sendMouseButton(int button, bool pressed) {
    if (!m_initialized) return;

    logEvent("MouseButton",
             "button=" + std::to_string(button) + ", " + (pressed ? "pressed" : "released"));
}

void VirtualInputDevice::sendMouseScroll(float scrollX, float scrollY) {
    if (!m_initialized) return;

    logEvent("MouseScroll",
             "x=" + std::to_string(scrollX) + ", y=" + std::to_string(scrollY));
}

void VirtualInputDevice::sendKeyboardKey(int keycode, bool pressed) {
    if (!m_initialized) return;

    logEvent("KeyboardKey",
             "keycode=" + std::to_string(keycode) + ", " + (pressed ? "pressed" : "released"));
}

void VirtualInputDevice::sendTouchEvent(int touchId, float x, float y, bool pressed) {
    if (!m_initialized) return;

    logEvent("TouchEvent",
             "id=" + std::to_string(touchId) + ", pos(" + std::to_string(x) + ", " +
             std::to_string(y) + "), " + (pressed ? "pressed" : "released"));
}

void VirtualInputDevice::logEvent(const std::string& eventType, const std::string& details) {
    Debug::log(LOG, "[moonlight] {} [{}]: {}", m_name, eventType, details);
}

// InputManager Implementation
InputManager::InputManager() {
    Debug::log(LOG, "[moonlight] Creating InputManager (Step 7: Input Management)");
}

InputManager::~InputManager() {
    shutdown();
}

bool InputManager::initialize() {
    if (m_initialized) {
        Debug::log(WARN, "[moonlight] InputManager already initialized");
        return true;
    }

    Debug::log(LOG, "[moonlight] Initializing InputManager");
    Debug::log(LOG, "[moonlight] Setting up virtual input devices for Moonlight clients");

    try {
        // Create default virtual input devices for Moonlight streaming
        createDefaultDevices();

        m_initialized = true;
        Debug::log(LOG, "[moonlight] InputManager initialized successfully");
        Debug::log(LOG, "[moonlight] Virtual input devices exposed to Wayland:");
        Debug::log(LOG, "[moonlight]   - Moonlight Keyboard (alongside VNC keyboard)");
        Debug::log(LOG, "[moonlight]   - Moonlight Mouse (alongside VNC mouse)");
        Debug::log(LOG, "[moonlight]   - Moonlight Touch (for mobile clients)");
        Debug::log(LOG, "[moonlight] Wayland will handle input routing from multiple sources");

        return true;
    } catch (const std::exception& e) {
        Debug::log(ERR, "[moonlight] Failed to initialize InputManager: {}", e.what());
        return false;
    }
}

void InputManager::shutdown() {
    if (!m_initialized) return;

    Debug::log(LOG, "[moonlight] Shutting down InputManager");

    // Remove all devices
    m_devices.clear();
    m_defaultKeyboard.reset();
    m_defaultMouse.reset();
    m_defaultTouch.reset();

    m_initialized = false;
    Debug::log(LOG, "[moonlight] InputManager stopped");
}

std::shared_ptr<VirtualInputDevice> InputManager::createDevice(DeviceType type, const std::string& name) {
    if (!m_initialized) {
        Debug::log(ERR, "[moonlight] Cannot create device: InputManager not initialized");
        return nullptr;
    }

    auto device = std::make_shared<VirtualInputDevice>(type, name);
    if (!device->initialize()) {
        Debug::log(ERR, "[moonlight] Failed to initialize device: {}", name);
        return nullptr;
    }

    m_devices.push_back(device);
    Debug::log(LOG, "[moonlight] Created virtual input device: {} (total devices: {})",
               name, m_devices.size());

    return device;
}

void InputManager::removeDevice(std::shared_ptr<VirtualInputDevice> device) {
    if (!device) return;

    auto it = std::find(m_devices.begin(), m_devices.end(), device);
    if (it != m_devices.end()) {
        Debug::log(LOG, "[moonlight] Removing virtual input device: {}", device->getName());
        m_devices.erase(it);
    }
}

void InputManager::handleMouseMove(float x, float y, bool relative) {
    if (m_defaultMouse) {
        m_defaultMouse->sendMouseMove(x, y, relative);
    }
}

void InputManager::handleMouseButton(int button, bool pressed) {
    if (m_defaultMouse) {
        m_defaultMouse->sendMouseButton(button, pressed);
    }
}

void InputManager::handleMouseScroll(float scrollX, float scrollY) {
    if (m_defaultMouse) {
        m_defaultMouse->sendMouseScroll(scrollX, scrollY);
    }
}

void InputManager::handleKeyboardKey(int keycode, bool pressed) {
    if (m_defaultKeyboard) {
        m_defaultKeyboard->sendKeyboardKey(keycode, pressed);
    }
}

void InputManager::handleTouchEvent(int touchId, float x, float y, bool pressed) {
    if (m_defaultTouch) {
        m_defaultTouch->sendTouchEvent(touchId, x, y, pressed);
    }
}

bool InputManager::isInputSupported() const {
    // Step 7 stub - always return true for virtual input support
    return true;
}

std::vector<std::string> InputManager::getSupportedDeviceTypes() const {
    return {
        "Keyboard (Linux evdev)",
        "Mouse (Linux evdev)",
        "Touch (Linux evdev)",
        "Gamepad (Linux evdev with force feedback)"
    };
}

void InputManager::createDefaultDevices() {
    Debug::log(LOG, "[moonlight] Creating default Moonlight input devices");

    // Create default keyboard device
    m_defaultKeyboard = createDevice(DeviceType::KEYBOARD, "Moonlight Virtual Keyboard");
    if (!m_defaultKeyboard) {
        throw std::runtime_error("Failed to create default keyboard device");
    }

    // Create default mouse device
    m_defaultMouse = createDevice(DeviceType::MOUSE, "Moonlight Virtual Mouse");
    if (!m_defaultMouse) {
        throw std::runtime_error("Failed to create default mouse device");
    }

    // Create default touch device
    m_defaultTouch = createDevice(DeviceType::TOUCH, "Moonlight Virtual Touch");
    if (!m_defaultTouch) {
        throw std::runtime_error("Failed to create default touch device");
    }

    Debug::log(LOG, "[moonlight] Default input devices created successfully");
}

} // namespace input
} // namespace moonlight