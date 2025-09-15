// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// cursor_shape_v1

/*
 This protocol's authors' copyright notice is:


    Copyright 2018 The Chromium Authors
    Copyright 2023 Simon Ser

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
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "cursor-shape-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wp_cursor_shape_manager_v1_interface;
extern const wl_interface wp_cursor_shape_device_v1_interface;
extern const wl_interface wl_pointer_interface;
extern const wl_interface zwp_tablet_tool_v2_interface;

static void _CWpCursorShapeManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpCursorShapeManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpCursorShapeManagerV1GetPointer(wl_client* client, wl_resource* resource, uint32_t cursor_shape_device, wl_resource* pointer) {
    const auto PO = (CWpCursorShapeManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getPointer)
        PO->requests.getPointer(PO, cursor_shape_device, pointer);
}

static void _CWpCursorShapeManagerV1GetTabletToolV2(wl_client* client, wl_resource* resource, uint32_t cursor_shape_device, wl_resource* tablet_tool) {
    const auto PO = (CWpCursorShapeManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getTabletToolV2)
        PO->requests.getTabletToolV2(PO, cursor_shape_device, tablet_tool);
}

static void _CWpCursorShapeManagerV1__DestroyListener(wl_listener* l, void* d) {
    CWpCursorShapeManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpCursorShapeManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpCursorShapeManagerV1VTable[] = {
    (void*)_CWpCursorShapeManagerV1Destroy,
    (void*)_CWpCursorShapeManagerV1GetPointer,
    (void*)_CWpCursorShapeManagerV1GetTabletToolV2,
};
static const wl_interface* _CWpCursorShapeManagerV1GetPointerTypes[] = {
    &wp_cursor_shape_device_v1_interface,
    &wl_pointer_interface,
};
static const wl_interface* _CWpCursorShapeManagerV1GetTabletToolV2Types[] = {
    &wp_cursor_shape_device_v1_interface,
    &zwp_tablet_tool_v2_interface,
};

static const wl_message _CWpCursorShapeManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_pointer", "no", _CWpCursorShapeManagerV1GetPointerTypes + 0},
    { "get_tablet_tool_v2", "no", _CWpCursorShapeManagerV1GetTabletToolV2Types + 0},
};

const wl_interface wp_cursor_shape_manager_v1_interface = {
    "wp_cursor_shape_manager_v1", 1,
    3, _CWpCursorShapeManagerV1Requests,
    0, nullptr,
};

CWpCursorShapeManagerV1::CWpCursorShapeManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_cursor_shape_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpCursorShapeManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpCursorShapeManagerV1VTable, this, nullptr);
}

CWpCursorShapeManagerV1::~CWpCursorShapeManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpCursorShapeManagerV1::onDestroyCalled() {
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

void CWpCursorShapeManagerV1::setDestroy(F<void(CWpCursorShapeManagerV1*)> handler) {
    requests.destroy = handler;
}

void CWpCursorShapeManagerV1::setGetPointer(F<void(CWpCursorShapeManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getPointer = handler;
}

void CWpCursorShapeManagerV1::setGetTabletToolV2(F<void(CWpCursorShapeManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getTabletToolV2 = handler;
}

static void _CWpCursorShapeDeviceV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpCursorShapeDeviceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpCursorShapeDeviceV1SetShape(wl_client* client, wl_resource* resource, uint32_t serial, wpCursorShapeDeviceV1Shape shape) {
    const auto PO = (CWpCursorShapeDeviceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setShape)
        PO->requests.setShape(PO, serial, shape);
}

static void _CWpCursorShapeDeviceV1__DestroyListener(wl_listener* l, void* d) {
    CWpCursorShapeDeviceV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpCursorShapeDeviceV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpCursorShapeDeviceV1VTable[] = {
    (void*)_CWpCursorShapeDeviceV1Destroy,
    (void*)_CWpCursorShapeDeviceV1SetShape,
};
static const wl_interface* _CWpCursorShapeDeviceV1SetShapeTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CWpCursorShapeDeviceV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_shape", "uu", _CWpCursorShapeDeviceV1SetShapeTypes + 0},
};

const wl_interface wp_cursor_shape_device_v1_interface = {
    "wp_cursor_shape_device_v1", 1,
    2, _CWpCursorShapeDeviceV1Requests,
    0, nullptr,
};

CWpCursorShapeDeviceV1::CWpCursorShapeDeviceV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_cursor_shape_device_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpCursorShapeDeviceV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpCursorShapeDeviceV1VTable, this, nullptr);
}

CWpCursorShapeDeviceV1::~CWpCursorShapeDeviceV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpCursorShapeDeviceV1::onDestroyCalled() {
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

void CWpCursorShapeDeviceV1::setDestroy(F<void(CWpCursorShapeDeviceV1*)> handler) {
    requests.destroy = handler;
}

void CWpCursorShapeDeviceV1::setSetShape(F<void(CWpCursorShapeDeviceV1*, uint32_t, wpCursorShapeDeviceV1Shape)> handler) {
    requests.setShape = handler;
}

#undef F
