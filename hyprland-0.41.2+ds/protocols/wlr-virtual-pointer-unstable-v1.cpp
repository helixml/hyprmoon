// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wlr_virtual_pointer_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2019 Josef Gajdusek

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice (including the next
    paragraph) shall be included in all copies or substantial portions of the
    Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "wlr-virtual-pointer-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwlr_virtual_pointer_v1_interface;
extern const wl_interface zwlr_virtual_pointer_manager_v1_interface;
extern const wl_interface wl_seat_interface;
extern const wl_interface wl_output_interface;

static void _CZwlrVirtualPointerV1Motion(wl_client* client, wl_resource* resource, uint32_t time, wl_fixed_t dx, wl_fixed_t dy) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.motion)
        PO->requests.motion(PO, time, dx, dy);
}

static void _CZwlrVirtualPointerV1MotionAbsolute(wl_client* client, wl_resource* resource, uint32_t time, uint32_t x, uint32_t y, uint32_t x_extent, uint32_t y_extent) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.motionAbsolute)
        PO->requests.motionAbsolute(PO, time, x, y, x_extent, y_extent);
}

static void _CZwlrVirtualPointerV1Button(wl_client* client, wl_resource* resource, uint32_t time, uint32_t button, uint32_t state) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.button)
        PO->requests.button(PO, time, button, state);
}

static void _CZwlrVirtualPointerV1Axis(wl_client* client, wl_resource* resource, uint32_t time, uint32_t axis, wl_fixed_t value) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.axis)
        PO->requests.axis(PO, time, axis, value);
}

static void _CZwlrVirtualPointerV1Frame(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.frame)
        PO->requests.frame(PO);
}

static void _CZwlrVirtualPointerV1AxisSource(wl_client* client, wl_resource* resource, uint32_t axis_source) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.axisSource)
        PO->requests.axisSource(PO, axis_source);
}

static void _CZwlrVirtualPointerV1AxisStop(wl_client* client, wl_resource* resource, uint32_t time, uint32_t axis) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.axisStop)
        PO->requests.axisStop(PO, time, axis);
}

static void _CZwlrVirtualPointerV1AxisDiscrete(wl_client* client, wl_resource* resource, uint32_t time, uint32_t axis, wl_fixed_t value, int32_t discrete) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.axisDiscrete)
        PO->requests.axisDiscrete(PO, time, axis, value, discrete);
}

static void _CZwlrVirtualPointerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrVirtualPointerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrVirtualPointerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrVirtualPointerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrVirtualPointerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrVirtualPointerV1VTable[] = {
    (void*)_CZwlrVirtualPointerV1Motion,
    (void*)_CZwlrVirtualPointerV1MotionAbsolute,
    (void*)_CZwlrVirtualPointerV1Button,
    (void*)_CZwlrVirtualPointerV1Axis,
    (void*)_CZwlrVirtualPointerV1Frame,
    (void*)_CZwlrVirtualPointerV1AxisSource,
    (void*)_CZwlrVirtualPointerV1AxisStop,
    (void*)_CZwlrVirtualPointerV1AxisDiscrete,
    (void*)_CZwlrVirtualPointerV1Destroy,
};
static const wl_interface* _CZwlrVirtualPointerV1MotionTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrVirtualPointerV1MotionAbsoluteTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrVirtualPointerV1ButtonTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrVirtualPointerV1AxisTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrVirtualPointerV1AxisSourceTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrVirtualPointerV1AxisStopTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrVirtualPointerV1AxisDiscreteTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwlrVirtualPointerV1Requests[] = {
    { "motion", "uff", _CZwlrVirtualPointerV1MotionTypes + 0},
    { "motion_absolute", "uuuuu", _CZwlrVirtualPointerV1MotionAbsoluteTypes + 0},
    { "button", "uuu", _CZwlrVirtualPointerV1ButtonTypes + 0},
    { "axis", "uuf", _CZwlrVirtualPointerV1AxisTypes + 0},
    { "frame", "", dummyTypes + 0},
    { "axis_source", "u", _CZwlrVirtualPointerV1AxisSourceTypes + 0},
    { "axis_stop", "uu", _CZwlrVirtualPointerV1AxisStopTypes + 0},
    { "axis_discrete", "uufi", _CZwlrVirtualPointerV1AxisDiscreteTypes + 0},
    { "destroy", "1", dummyTypes + 0},
};

const wl_interface zwlr_virtual_pointer_v1_interface = {
    "zwlr_virtual_pointer_v1", 2,
    9, _CZwlrVirtualPointerV1Requests,
    0, nullptr,
};

CZwlrVirtualPointerV1::CZwlrVirtualPointerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_virtual_pointer_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrVirtualPointerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrVirtualPointerV1VTable, this, nullptr);
}

CZwlrVirtualPointerV1::~CZwlrVirtualPointerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrVirtualPointerV1::onDestroyCalled() {
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

void CZwlrVirtualPointerV1::setMotion(F<void(CZwlrVirtualPointerV1*, uint32_t, wl_fixed_t, wl_fixed_t)> handler) {
    requests.motion = handler;
}

void CZwlrVirtualPointerV1::setMotionAbsolute(F<void(CZwlrVirtualPointerV1*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)> handler) {
    requests.motionAbsolute = handler;
}

void CZwlrVirtualPointerV1::setButton(F<void(CZwlrVirtualPointerV1*, uint32_t, uint32_t, uint32_t)> handler) {
    requests.button = handler;
}

void CZwlrVirtualPointerV1::setAxis(F<void(CZwlrVirtualPointerV1*, uint32_t, uint32_t, wl_fixed_t)> handler) {
    requests.axis = handler;
}

void CZwlrVirtualPointerV1::setFrame(F<void(CZwlrVirtualPointerV1*)> handler) {
    requests.frame = handler;
}

void CZwlrVirtualPointerV1::setAxisSource(F<void(CZwlrVirtualPointerV1*, uint32_t)> handler) {
    requests.axisSource = handler;
}

void CZwlrVirtualPointerV1::setAxisStop(F<void(CZwlrVirtualPointerV1*, uint32_t, uint32_t)> handler) {
    requests.axisStop = handler;
}

void CZwlrVirtualPointerV1::setAxisDiscrete(F<void(CZwlrVirtualPointerV1*, uint32_t, uint32_t, wl_fixed_t, int32_t)> handler) {
    requests.axisDiscrete = handler;
}

void CZwlrVirtualPointerV1::setDestroy(F<void(CZwlrVirtualPointerV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwlrVirtualPointerManagerV1CreateVirtualPointer(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t id) {
    const auto PO = (CZwlrVirtualPointerManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createVirtualPointer)
        PO->requests.createVirtualPointer(PO, seat, id);
}

static void _CZwlrVirtualPointerManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrVirtualPointerManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrVirtualPointerManagerV1CreateVirtualPointerWithOutput(wl_client* client, wl_resource* resource, wl_resource* seat, wl_resource* output, uint32_t id) {
    const auto PO = (CZwlrVirtualPointerManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createVirtualPointerWithOutput)
        PO->requests.createVirtualPointerWithOutput(PO, seat, output, id);
}

static void _CZwlrVirtualPointerManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrVirtualPointerManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrVirtualPointerManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrVirtualPointerManagerV1VTable[] = {
    (void*)_CZwlrVirtualPointerManagerV1CreateVirtualPointer,
    (void*)_CZwlrVirtualPointerManagerV1Destroy,
    (void*)_CZwlrVirtualPointerManagerV1CreateVirtualPointerWithOutput,
};
static const wl_interface* _CZwlrVirtualPointerManagerV1CreateVirtualPointerTypes[] = {
    &wl_seat_interface,
    &zwlr_virtual_pointer_v1_interface,
};
static const wl_interface* _CZwlrVirtualPointerManagerV1CreateVirtualPointerWithOutputTypes[] = {
    &wl_seat_interface,
    &wl_output_interface,
    &zwlr_virtual_pointer_v1_interface,
};

static const wl_message _CZwlrVirtualPointerManagerV1Requests[] = {
    { "create_virtual_pointer", "?on", _CZwlrVirtualPointerManagerV1CreateVirtualPointerTypes + 0},
    { "destroy", "1", dummyTypes + 0},
    { "create_virtual_pointer_with_output", "2?o?on", _CZwlrVirtualPointerManagerV1CreateVirtualPointerWithOutputTypes + 0},
};

const wl_interface zwlr_virtual_pointer_manager_v1_interface = {
    "zwlr_virtual_pointer_manager_v1", 2,
    3, _CZwlrVirtualPointerManagerV1Requests,
    0, nullptr,
};

CZwlrVirtualPointerManagerV1::CZwlrVirtualPointerManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_virtual_pointer_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrVirtualPointerManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrVirtualPointerManagerV1VTable, this, nullptr);
}

CZwlrVirtualPointerManagerV1::~CZwlrVirtualPointerManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrVirtualPointerManagerV1::onDestroyCalled() {
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

void CZwlrVirtualPointerManagerV1::setCreateVirtualPointer(F<void(CZwlrVirtualPointerManagerV1*, wl_resource*, uint32_t)> handler) {
    requests.createVirtualPointer = handler;
}

void CZwlrVirtualPointerManagerV1::setDestroy(F<void(CZwlrVirtualPointerManagerV1*)> handler) {
    requests.destroy = handler;
}

void CZwlrVirtualPointerManagerV1::setCreateVirtualPointerWithOutput(F<void(CZwlrVirtualPointerManagerV1*, wl_resource*, wl_resource*, uint32_t)> handler) {
    requests.createVirtualPointerWithOutput = handler;
}

#undef F
