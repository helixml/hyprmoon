// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// tablet_v2

/*
 This protocol's authors' copyright notice is:


    Copyright 2014 © Stephen "Lyude" Chandler Paul
    Copyright 2015-2016 © Red Hat, Inc.

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice (including the
    next paragraph) shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
    BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "tablet-v2.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_tablet_manager_v2_interface;
extern const wl_interface zwp_tablet_seat_v2_interface;
extern const wl_interface zwp_tablet_tool_v2_interface;
extern const wl_interface zwp_tablet_v2_interface;
extern const wl_interface zwp_tablet_pad_ring_v2_interface;
extern const wl_interface zwp_tablet_pad_strip_v2_interface;
extern const wl_interface zwp_tablet_pad_group_v2_interface;
extern const wl_interface zwp_tablet_pad_v2_interface;
extern const wl_interface wl_seat_interface;
extern const wl_interface wl_surface_interface;

static void _CZwpTabletManagerV2GetTabletSeat(wl_client* client, wl_resource* resource, uint32_t tablet_seat, wl_resource* seat) {
    const auto PO = (CZwpTabletManagerV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getTabletSeat)
        PO->requests.getTabletSeat(PO, tablet_seat, seat);
}

static void _CZwpTabletManagerV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletManagerV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletManagerV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletManagerV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletManagerV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletManagerV2VTable[] = {
    (void*)_CZwpTabletManagerV2GetTabletSeat,
    (void*)_CZwpTabletManagerV2Destroy,
};
static const wl_interface* _CZwpTabletManagerV2GetTabletSeatTypes[] = {
    &zwp_tablet_seat_v2_interface,
    &wl_seat_interface,
};

static const wl_message _CZwpTabletManagerV2Requests[] = {
    { "get_tablet_seat", "no", _CZwpTabletManagerV2GetTabletSeatTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface zwp_tablet_manager_v2_interface = {
    "zwp_tablet_manager_v2", 1,
    2, _CZwpTabletManagerV2Requests,
    0, nullptr,
};

CZwpTabletManagerV2::CZwpTabletManagerV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_manager_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletManagerV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletManagerV2VTable, this, nullptr);
}

CZwpTabletManagerV2::~CZwpTabletManagerV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletManagerV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletManagerV2::setGetTabletSeat(F<void(CZwpTabletManagerV2*, uint32_t, wl_resource*)> handler) {
    requests.getTabletSeat = handler;
}

void CZwpTabletManagerV2::setDestroy(F<void(CZwpTabletManagerV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletSeatV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletSeatV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletSeatV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletSeatV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletSeatV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletSeatV2VTable[] = {
    (void*)_CZwpTabletSeatV2Destroy,
};

void CZwpTabletSeatV2::sendTabletAdded(CZwpTabletV2* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, id ? id->pResource : nullptr);
}

void CZwpTabletSeatV2::sendToolAdded(CZwpTabletToolV2* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, id ? id->pResource : nullptr);
}

void CZwpTabletSeatV2::sendPadAdded(CZwpTabletPadV2* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, id ? id->pResource : nullptr);
}

void CZwpTabletSeatV2::sendTabletAddedRaw(CZwpTabletV2* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, id);
}

void CZwpTabletSeatV2::sendToolAddedRaw(CZwpTabletToolV2* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, id);
}

void CZwpTabletSeatV2::sendPadAddedRaw(CZwpTabletPadV2* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, id);
}
static const wl_interface* _CZwpTabletSeatV2TabletAddedTypes[] = {
    &zwp_tablet_v2_interface,
};
static const wl_interface* _CZwpTabletSeatV2ToolAddedTypes[] = {
    &zwp_tablet_tool_v2_interface,
};
static const wl_interface* _CZwpTabletSeatV2PadAddedTypes[] = {
    &zwp_tablet_pad_v2_interface,
};

static const wl_message _CZwpTabletSeatV2Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletSeatV2Events[] = {
    { "tablet_added", "n", _CZwpTabletSeatV2TabletAddedTypes + 0},
    { "tool_added", "n", _CZwpTabletSeatV2ToolAddedTypes + 0},
    { "pad_added", "n", _CZwpTabletSeatV2PadAddedTypes + 0},
};

const wl_interface zwp_tablet_seat_v2_interface = {
    "zwp_tablet_seat_v2", 1,
    1, _CZwpTabletSeatV2Requests,
    3, _CZwpTabletSeatV2Events,
};

CZwpTabletSeatV2::CZwpTabletSeatV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_seat_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletSeatV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletSeatV2VTable, this, nullptr);
}

CZwpTabletSeatV2::~CZwpTabletSeatV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletSeatV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletSeatV2::setDestroy(F<void(CZwpTabletSeatV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletToolV2SetCursor(wl_client* client, wl_resource* resource, uint32_t serial, wl_resource* surface, int32_t hotspot_x, int32_t hotspot_y) {
    const auto PO = (CZwpTabletToolV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setCursor)
        PO->requests.setCursor(PO, serial, surface, hotspot_x, hotspot_y);
}

static void _CZwpTabletToolV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletToolV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletToolV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletToolV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletToolV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletToolV2VTable[] = {
    (void*)_CZwpTabletToolV2SetCursor,
    (void*)_CZwpTabletToolV2Destroy,
};

void CZwpTabletToolV2::sendType(zwpTabletToolV2Type tool_type) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, tool_type);
}

void CZwpTabletToolV2::sendHardwareSerial(uint32_t hardware_serial_hi, uint32_t hardware_serial_lo) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, hardware_serial_hi, hardware_serial_lo);
}

void CZwpTabletToolV2::sendHardwareIdWacom(uint32_t hardware_id_hi, uint32_t hardware_id_lo) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, hardware_id_hi, hardware_id_lo);
}

void CZwpTabletToolV2::sendCapability(zwpTabletToolV2Capability capability) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, capability);
}

void CZwpTabletToolV2::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CZwpTabletToolV2::sendRemoved() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CZwpTabletToolV2::sendProximityIn(uint32_t serial, CZwpTabletV2* tablet, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, serial, tablet ? tablet->pResource : nullptr, surface);
}

void CZwpTabletToolV2::sendProximityOut() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7);
}

void CZwpTabletToolV2::sendDown(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 8, serial);
}

void CZwpTabletToolV2::sendUp() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 9);
}

void CZwpTabletToolV2::sendMotion(wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 10, x, y);
}

void CZwpTabletToolV2::sendPressure(uint32_t pressure) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 11, pressure);
}

void CZwpTabletToolV2::sendDistance(uint32_t distance) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 12, distance);
}

void CZwpTabletToolV2::sendTilt(wl_fixed_t tilt_x, wl_fixed_t tilt_y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 13, tilt_x, tilt_y);
}

void CZwpTabletToolV2::sendRotation(wl_fixed_t degrees) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 14, degrees);
}

void CZwpTabletToolV2::sendSlider(int32_t position) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 15, position);
}

void CZwpTabletToolV2::sendWheel(wl_fixed_t degrees, int32_t clicks) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 16, degrees, clicks);
}

void CZwpTabletToolV2::sendButton(uint32_t serial, uint32_t button, zwpTabletToolV2ButtonState state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 17, serial, button, state);
}

void CZwpTabletToolV2::sendFrame(uint32_t time) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 18, time);
}

void CZwpTabletToolV2::sendTypeRaw(zwpTabletToolV2Type tool_type) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, tool_type);
}

void CZwpTabletToolV2::sendHardwareSerialRaw(uint32_t hardware_serial_hi, uint32_t hardware_serial_lo) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, hardware_serial_hi, hardware_serial_lo);
}

void CZwpTabletToolV2::sendHardwareIdWacomRaw(uint32_t hardware_id_hi, uint32_t hardware_id_lo) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, hardware_id_hi, hardware_id_lo);
}

void CZwpTabletToolV2::sendCapabilityRaw(zwpTabletToolV2Capability capability) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, capability);
}

void CZwpTabletToolV2::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CZwpTabletToolV2::sendRemovedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CZwpTabletToolV2::sendProximityInRaw(uint32_t serial, wl_resource* tablet, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, serial, tablet, surface);
}

void CZwpTabletToolV2::sendProximityOutRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7);
}

void CZwpTabletToolV2::sendDownRaw(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 8, serial);
}

void CZwpTabletToolV2::sendUpRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 9);
}

void CZwpTabletToolV2::sendMotionRaw(wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 10, x, y);
}

void CZwpTabletToolV2::sendPressureRaw(uint32_t pressure) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 11, pressure);
}

void CZwpTabletToolV2::sendDistanceRaw(uint32_t distance) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 12, distance);
}

void CZwpTabletToolV2::sendTiltRaw(wl_fixed_t tilt_x, wl_fixed_t tilt_y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 13, tilt_x, tilt_y);
}

void CZwpTabletToolV2::sendRotationRaw(wl_fixed_t degrees) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 14, degrees);
}

void CZwpTabletToolV2::sendSliderRaw(int32_t position) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 15, position);
}

void CZwpTabletToolV2::sendWheelRaw(wl_fixed_t degrees, int32_t clicks) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 16, degrees, clicks);
}

void CZwpTabletToolV2::sendButtonRaw(uint32_t serial, uint32_t button, zwpTabletToolV2ButtonState state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 17, serial, button, state);
}

void CZwpTabletToolV2::sendFrameRaw(uint32_t time) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 18, time);
}
static const wl_interface* _CZwpTabletToolV2SetCursorTypes[] = {
    nullptr,
    &wl_surface_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2TypeTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2HardwareSerialTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2HardwareIdWacomTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2CapabilityTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2ProximityInTypes[] = {
    nullptr,
    &zwp_tablet_v2_interface,
    &wl_surface_interface,
};
static const wl_interface* _CZwpTabletToolV2DownTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2MotionTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2PressureTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2DistanceTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2TiltTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2RotationTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2SliderTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2WheelTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2ButtonTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletToolV2FrameTypes[] = {
    nullptr,
};

static const wl_message _CZwpTabletToolV2Requests[] = {
    { "set_cursor", "u?oii", _CZwpTabletToolV2SetCursorTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletToolV2Events[] = {
    { "type", "u", _CZwpTabletToolV2TypeTypes + 0},
    { "hardware_serial", "uu", _CZwpTabletToolV2HardwareSerialTypes + 0},
    { "hardware_id_wacom", "uu", _CZwpTabletToolV2HardwareIdWacomTypes + 0},
    { "capability", "u", _CZwpTabletToolV2CapabilityTypes + 0},
    { "done", "", dummyTypes + 0},
    { "removed", "", dummyTypes + 0},
    { "proximity_in", "uoo", _CZwpTabletToolV2ProximityInTypes + 0},
    { "proximity_out", "", dummyTypes + 0},
    { "down", "u", _CZwpTabletToolV2DownTypes + 0},
    { "up", "", dummyTypes + 0},
    { "motion", "ff", _CZwpTabletToolV2MotionTypes + 0},
    { "pressure", "u", _CZwpTabletToolV2PressureTypes + 0},
    { "distance", "u", _CZwpTabletToolV2DistanceTypes + 0},
    { "tilt", "ff", _CZwpTabletToolV2TiltTypes + 0},
    { "rotation", "f", _CZwpTabletToolV2RotationTypes + 0},
    { "slider", "i", _CZwpTabletToolV2SliderTypes + 0},
    { "wheel", "fi", _CZwpTabletToolV2WheelTypes + 0},
    { "button", "uuu", _CZwpTabletToolV2ButtonTypes + 0},
    { "frame", "u", _CZwpTabletToolV2FrameTypes + 0},
};

const wl_interface zwp_tablet_tool_v2_interface = {
    "zwp_tablet_tool_v2", 1,
    2, _CZwpTabletToolV2Requests,
    19, _CZwpTabletToolV2Events,
};

CZwpTabletToolV2::CZwpTabletToolV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_tool_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletToolV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletToolV2VTable, this, nullptr);
}

CZwpTabletToolV2::~CZwpTabletToolV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletToolV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletToolV2::setSetCursor(F<void(CZwpTabletToolV2*, uint32_t, wl_resource*, int32_t, int32_t)> handler) {
    requests.setCursor = handler;
}

void CZwpTabletToolV2::setDestroy(F<void(CZwpTabletToolV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletV2VTable[] = {
    (void*)_CZwpTabletV2Destroy,
};

void CZwpTabletV2::sendName(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name);
}

void CZwpTabletV2::sendId(uint32_t vid, uint32_t pid) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, vid, pid);
}

void CZwpTabletV2::sendPath(const char* path) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, path);
}

void CZwpTabletV2::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CZwpTabletV2::sendRemoved() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CZwpTabletV2::sendNameRaw(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name);
}

void CZwpTabletV2::sendIdRaw(uint32_t vid, uint32_t pid) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, vid, pid);
}

void CZwpTabletV2::sendPathRaw(const char* path) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, path);
}

void CZwpTabletV2::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CZwpTabletV2::sendRemovedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}
static const wl_interface* _CZwpTabletV2NameTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletV2IdTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletV2PathTypes[] = {
    nullptr,
};

static const wl_message _CZwpTabletV2Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletV2Events[] = {
    { "name", "s", _CZwpTabletV2NameTypes + 0},
    { "id", "uu", _CZwpTabletV2IdTypes + 0},
    { "path", "s", _CZwpTabletV2PathTypes + 0},
    { "done", "", dummyTypes + 0},
    { "removed", "", dummyTypes + 0},
};

const wl_interface zwp_tablet_v2_interface = {
    "zwp_tablet_v2", 1,
    1, _CZwpTabletV2Requests,
    5, _CZwpTabletV2Events,
};

CZwpTabletV2::CZwpTabletV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletV2VTable, this, nullptr);
}

CZwpTabletV2::~CZwpTabletV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletV2::setDestroy(F<void(CZwpTabletV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletPadRingV2SetFeedback(wl_client* client, wl_resource* resource, const char* description, uint32_t serial) {
    const auto PO = (CZwpTabletPadRingV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setFeedback)
        PO->requests.setFeedback(PO, description, serial);
}

static void _CZwpTabletPadRingV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletPadRingV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletPadRingV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletPadRingV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletPadRingV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletPadRingV2VTable[] = {
    (void*)_CZwpTabletPadRingV2SetFeedback,
    (void*)_CZwpTabletPadRingV2Destroy,
};

void CZwpTabletPadRingV2::sendSource(zwpTabletPadRingV2Source source) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, source);
}

void CZwpTabletPadRingV2::sendAngle(wl_fixed_t degrees) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, degrees);
}

void CZwpTabletPadRingV2::sendStop() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwpTabletPadRingV2::sendFrame(uint32_t time) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, time);
}

void CZwpTabletPadRingV2::sendSourceRaw(zwpTabletPadRingV2Source source) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, source);
}

void CZwpTabletPadRingV2::sendAngleRaw(wl_fixed_t degrees) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, degrees);
}

void CZwpTabletPadRingV2::sendStopRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwpTabletPadRingV2::sendFrameRaw(uint32_t time) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, time);
}
static const wl_interface* _CZwpTabletPadRingV2SetFeedbackTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletPadRingV2SourceTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadRingV2AngleTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadRingV2FrameTypes[] = {
    nullptr,
};

static const wl_message _CZwpTabletPadRingV2Requests[] = {
    { "set_feedback", "su", _CZwpTabletPadRingV2SetFeedbackTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletPadRingV2Events[] = {
    { "source", "u", _CZwpTabletPadRingV2SourceTypes + 0},
    { "angle", "f", _CZwpTabletPadRingV2AngleTypes + 0},
    { "stop", "", dummyTypes + 0},
    { "frame", "u", _CZwpTabletPadRingV2FrameTypes + 0},
};

const wl_interface zwp_tablet_pad_ring_v2_interface = {
    "zwp_tablet_pad_ring_v2", 1,
    2, _CZwpTabletPadRingV2Requests,
    4, _CZwpTabletPadRingV2Events,
};

CZwpTabletPadRingV2::CZwpTabletPadRingV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_pad_ring_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletPadRingV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletPadRingV2VTable, this, nullptr);
}

CZwpTabletPadRingV2::~CZwpTabletPadRingV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletPadRingV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletPadRingV2::setSetFeedback(F<void(CZwpTabletPadRingV2*, const char*, uint32_t)> handler) {
    requests.setFeedback = handler;
}

void CZwpTabletPadRingV2::setDestroy(F<void(CZwpTabletPadRingV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletPadStripV2SetFeedback(wl_client* client, wl_resource* resource, const char* description, uint32_t serial) {
    const auto PO = (CZwpTabletPadStripV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setFeedback)
        PO->requests.setFeedback(PO, description, serial);
}

static void _CZwpTabletPadStripV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletPadStripV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletPadStripV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletPadStripV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletPadStripV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletPadStripV2VTable[] = {
    (void*)_CZwpTabletPadStripV2SetFeedback,
    (void*)_CZwpTabletPadStripV2Destroy,
};

void CZwpTabletPadStripV2::sendSource(zwpTabletPadRingV2Source source) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, source);
}

void CZwpTabletPadStripV2::sendPosition(uint32_t position) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, position);
}

void CZwpTabletPadStripV2::sendStop() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwpTabletPadStripV2::sendFrame(uint32_t time) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, time);
}

void CZwpTabletPadStripV2::sendSourceRaw(zwpTabletPadRingV2Source source) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, source);
}

void CZwpTabletPadStripV2::sendPositionRaw(uint32_t position) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, position);
}

void CZwpTabletPadStripV2::sendStopRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwpTabletPadStripV2::sendFrameRaw(uint32_t time) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, time);
}
static const wl_interface* _CZwpTabletPadStripV2SetFeedbackTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletPadStripV2SourceTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadStripV2PositionTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadStripV2FrameTypes[] = {
    nullptr,
};

static const wl_message _CZwpTabletPadStripV2Requests[] = {
    { "set_feedback", "su", _CZwpTabletPadStripV2SetFeedbackTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletPadStripV2Events[] = {
    { "source", "u", _CZwpTabletPadStripV2SourceTypes + 0},
    { "position", "u", _CZwpTabletPadStripV2PositionTypes + 0},
    { "stop", "", dummyTypes + 0},
    { "frame", "u", _CZwpTabletPadStripV2FrameTypes + 0},
};

const wl_interface zwp_tablet_pad_strip_v2_interface = {
    "zwp_tablet_pad_strip_v2", 1,
    2, _CZwpTabletPadStripV2Requests,
    4, _CZwpTabletPadStripV2Events,
};

CZwpTabletPadStripV2::CZwpTabletPadStripV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_pad_strip_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletPadStripV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletPadStripV2VTable, this, nullptr);
}

CZwpTabletPadStripV2::~CZwpTabletPadStripV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletPadStripV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletPadStripV2::setSetFeedback(F<void(CZwpTabletPadStripV2*, const char*, uint32_t)> handler) {
    requests.setFeedback = handler;
}

void CZwpTabletPadStripV2::setDestroy(F<void(CZwpTabletPadStripV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletPadGroupV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletPadGroupV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletPadGroupV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletPadGroupV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletPadGroupV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletPadGroupV2VTable[] = {
    (void*)_CZwpTabletPadGroupV2Destroy,
};

void CZwpTabletPadGroupV2::sendButtons(wl_array* buttons) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, buttons);
}

void CZwpTabletPadGroupV2::sendRing(CZwpTabletPadRingV2* ring) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, ring ? ring->pResource : nullptr);
}

void CZwpTabletPadGroupV2::sendStrip(CZwpTabletPadStripV2* strip) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, strip ? strip->pResource : nullptr);
}

void CZwpTabletPadGroupV2::sendModes(uint32_t modes) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, modes);
}

void CZwpTabletPadGroupV2::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CZwpTabletPadGroupV2::sendModeSwitch(uint32_t time, uint32_t serial, uint32_t mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, time, serial, mode);
}

void CZwpTabletPadGroupV2::sendButtonsRaw(wl_array* buttons) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, buttons);
}

void CZwpTabletPadGroupV2::sendRingRaw(CZwpTabletPadRingV2* ring) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, ring);
}

void CZwpTabletPadGroupV2::sendStripRaw(CZwpTabletPadStripV2* strip) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, strip);
}

void CZwpTabletPadGroupV2::sendModesRaw(uint32_t modes) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, modes);
}

void CZwpTabletPadGroupV2::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CZwpTabletPadGroupV2::sendModeSwitchRaw(uint32_t time, uint32_t serial, uint32_t mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, time, serial, mode);
}
static const wl_interface* _CZwpTabletPadGroupV2ButtonsTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadGroupV2RingTypes[] = {
    &zwp_tablet_pad_ring_v2_interface,
};
static const wl_interface* _CZwpTabletPadGroupV2StripTypes[] = {
    &zwp_tablet_pad_strip_v2_interface,
};
static const wl_interface* _CZwpTabletPadGroupV2ModesTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadGroupV2ModeSwitchTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwpTabletPadGroupV2Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletPadGroupV2Events[] = {
    { "buttons", "a", _CZwpTabletPadGroupV2ButtonsTypes + 0},
    { "ring", "n", _CZwpTabletPadGroupV2RingTypes + 0},
    { "strip", "n", _CZwpTabletPadGroupV2StripTypes + 0},
    { "modes", "u", _CZwpTabletPadGroupV2ModesTypes + 0},
    { "done", "", dummyTypes + 0},
    { "mode_switch", "uuu", _CZwpTabletPadGroupV2ModeSwitchTypes + 0},
};

const wl_interface zwp_tablet_pad_group_v2_interface = {
    "zwp_tablet_pad_group_v2", 1,
    1, _CZwpTabletPadGroupV2Requests,
    6, _CZwpTabletPadGroupV2Events,
};

CZwpTabletPadGroupV2::CZwpTabletPadGroupV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_pad_group_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletPadGroupV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletPadGroupV2VTable, this, nullptr);
}

CZwpTabletPadGroupV2::~CZwpTabletPadGroupV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletPadGroupV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletPadGroupV2::setDestroy(F<void(CZwpTabletPadGroupV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpTabletPadV2SetFeedback(wl_client* client, wl_resource* resource, uint32_t button, const char* description, uint32_t serial) {
    const auto PO = (CZwpTabletPadV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setFeedback)
        PO->requests.setFeedback(PO, button, description, serial);
}

static void _CZwpTabletPadV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTabletPadV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTabletPadV2__DestroyListener(wl_listener* l, void* d) {
    CZwpTabletPadV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTabletPadV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTabletPadV2VTable[] = {
    (void*)_CZwpTabletPadV2SetFeedback,
    (void*)_CZwpTabletPadV2Destroy,
};

void CZwpTabletPadV2::sendGroup(CZwpTabletPadGroupV2* pad_group) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, pad_group ? pad_group->pResource : nullptr);
}

void CZwpTabletPadV2::sendPath(const char* path) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, path);
}

void CZwpTabletPadV2::sendButtons(uint32_t buttons) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, buttons);
}

void CZwpTabletPadV2::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CZwpTabletPadV2::sendButton(uint32_t time, uint32_t button, zwpTabletToolV2ButtonState state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, time, button, state);
}

void CZwpTabletPadV2::sendEnter(uint32_t serial, CZwpTabletV2* tablet, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, serial, tablet ? tablet->pResource : nullptr, surface);
}

void CZwpTabletPadV2::sendLeave(uint32_t serial, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, serial, surface);
}

void CZwpTabletPadV2::sendRemoved() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7);
}

void CZwpTabletPadV2::sendGroupRaw(CZwpTabletPadGroupV2* pad_group) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, pad_group);
}

void CZwpTabletPadV2::sendPathRaw(const char* path) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, path);
}

void CZwpTabletPadV2::sendButtonsRaw(uint32_t buttons) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, buttons);
}

void CZwpTabletPadV2::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CZwpTabletPadV2::sendButtonRaw(uint32_t time, uint32_t button, zwpTabletToolV2ButtonState state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, time, button, state);
}

void CZwpTabletPadV2::sendEnterRaw(uint32_t serial, wl_resource* tablet, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, serial, tablet, surface);
}

void CZwpTabletPadV2::sendLeaveRaw(uint32_t serial, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, serial, surface);
}

void CZwpTabletPadV2::sendRemovedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7);
}
static const wl_interface* _CZwpTabletPadV2SetFeedbackTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletPadV2GroupTypes[] = {
    &zwp_tablet_pad_group_v2_interface,
};
static const wl_interface* _CZwpTabletPadV2PathTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadV2ButtonsTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTabletPadV2ButtonTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTabletPadV2EnterTypes[] = {
    nullptr,
    &zwp_tablet_v2_interface,
    &wl_surface_interface,
};
static const wl_interface* _CZwpTabletPadV2LeaveTypes[] = {
    nullptr,
    &wl_surface_interface,
};

static const wl_message _CZwpTabletPadV2Requests[] = {
    { "set_feedback", "usu", _CZwpTabletPadV2SetFeedbackTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpTabletPadV2Events[] = {
    { "group", "n", _CZwpTabletPadV2GroupTypes + 0},
    { "path", "s", _CZwpTabletPadV2PathTypes + 0},
    { "buttons", "u", _CZwpTabletPadV2ButtonsTypes + 0},
    { "done", "", dummyTypes + 0},
    { "button", "uuu", _CZwpTabletPadV2ButtonTypes + 0},
    { "enter", "uoo", _CZwpTabletPadV2EnterTypes + 0},
    { "leave", "uo", _CZwpTabletPadV2LeaveTypes + 0},
    { "removed", "", dummyTypes + 0},
};

const wl_interface zwp_tablet_pad_v2_interface = {
    "zwp_tablet_pad_v2", 1,
    2, _CZwpTabletPadV2Requests,
    8, _CZwpTabletPadV2Events,
};

CZwpTabletPadV2::CZwpTabletPadV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_tablet_pad_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTabletPadV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTabletPadV2VTable, this, nullptr);
}

CZwpTabletPadV2::~CZwpTabletPadV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTabletPadV2::onDestroyCalled() {
    wl_resource_set_user_data(pResource, nullptr);
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // set the resource to nullptr,
    // as it will be freed. If the consumer does not destroy this resource
    // in onDestroy here, we'd be doing a UAF in the ~dtor
    pResource = nullptr;

    if (onDestroy)
        onDestroy(this);
}

void CZwpTabletPadV2::setSetFeedback(F<void(CZwpTabletPadV2*, uint32_t, const char*, uint32_t)> handler) {
    requests.setFeedback = handler;
}

void CZwpTabletPadV2::setDestroy(F<void(CZwpTabletPadV2*)> handler) {
    requests.destroy = handler;
}

#undef F
