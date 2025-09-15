#pragma once

#include "../../debug/Log.hpp"
#include <memory>
#include <string>
#include <vector>

namespace moonlight {
namespace input {

// Forward declarations
struct MouseEvent;
struct KeyboardEvent;
struct TouchEvent;

// Input device types
enum class DeviceType {
    KEYBOARD = 0,
    MOUSE = 1,
    TOUCH = 2,
    GAMEPAD = 3
};

// Virtual input device representation
class VirtualInputDevice {
public:
    VirtualInputDevice(DeviceType type, const std::string& name);
    ~VirtualInputDevice();

    bool initialize();
    void shutdown();

    // Input event methods
    void sendMouseMove(float x, float y, bool relative = false);
    void sendMouseButton(int button, bool pressed);
    void sendMouseScroll(float scrollX, float scrollY);
    void sendKeyboardKey(int keycode, bool pressed);
    void sendTouchEvent(int touchId, float x, float y, bool pressed);

    bool isInitialized() const { return m_initialized; }
    DeviceType getType() const { return m_type; }
    const std::string& getName() const { return m_name; }

private:
    DeviceType m_type;
    std::string m_name;
    bool m_initialized = false;

    // Stub implementation for Step 7
    void logEvent(const std::string& eventType, const std::string& details);
};

// Main input manager for Moonlight
class InputManager {
public:
    InputManager();
    ~InputManager();

    bool initialize();
    void shutdown();

    // Device management
    std::shared_ptr<VirtualInputDevice> createDevice(DeviceType type, const std::string& name);
    void removeDevice(std::shared_ptr<VirtualInputDevice> device);

    // Input routing from Moonlight clients
    void handleMouseMove(float x, float y, bool relative = false);
    void handleMouseButton(int button, bool pressed);
    void handleMouseScroll(float scrollX, float scrollY);
    void handleKeyboardKey(int keycode, bool pressed);
    void handleTouchEvent(int touchId, float x, float y, bool pressed);

    // Platform abstraction
    bool isInputSupported() const;
    std::vector<std::string> getSupportedDeviceTypes() const;

    bool isInitialized() const { return m_initialized; }

private:
    bool m_initialized = false;
    std::vector<std::shared_ptr<VirtualInputDevice>> m_devices;

    // Default devices for Moonlight streaming
    std::shared_ptr<VirtualInputDevice> m_defaultKeyboard;
    std::shared_ptr<VirtualInputDevice> m_defaultMouse;
    std::shared_ptr<VirtualInputDevice> m_defaultTouch;

    void createDefaultDevices();
};

} // namespace input
} // namespace moonlight