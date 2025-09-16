// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// xwayland_shell_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2022 Joshua Ashton

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
#include "xwayland-shell-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface xwayland_shell_v1_interface;
extern const wl_interface xwayland_surface_v1_interface;
extern const wl_interface wl_surface_interface;

static void _CXwaylandShellV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXwaylandShellV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXwaylandShellV1GetXwaylandSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CXwaylandShellV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getXwaylandSurface)
        PO->requests.getXwaylandSurface(PO, id, surface);
}

static void _CXwaylandShellV1__DestroyListener(wl_listener* l, void* d) {
    CXwaylandShellV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXwaylandShellV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXwaylandShellV1VTable[] = {
    (void*)_CXwaylandShellV1Destroy,
    (void*)_CXwaylandShellV1GetXwaylandSurface,
};
static const wl_interface* _CXwaylandShellV1GetXwaylandSurfaceTypes[] = {
    &xwayland_surface_v1_interface,
    &wl_surface_interface,
};

static const wl_message _CXwaylandShellV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_xwayland_surface", "no", _CXwaylandShellV1GetXwaylandSurfaceTypes + 0},
};

const wl_interface xwayland_shell_v1_interface = {
    "xwayland_shell_v1", 1,
    2, _CXwaylandShellV1Requests,
    0, nullptr,
};

CXwaylandShellV1::CXwaylandShellV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xwayland_shell_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXwaylandShellV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXwaylandShellV1VTable, this, nullptr);
}

CXwaylandShellV1::~CXwaylandShellV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXwaylandShellV1::onDestroyCalled() {
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

void CXwaylandShellV1::setDestroy(F<void(CXwaylandShellV1*)> handler) {
    requests.destroy = handler;
}

void CXwaylandShellV1::setGetXwaylandSurface(F<void(CXwaylandShellV1*, uint32_t, wl_resource*)> handler) {
    requests.getXwaylandSurface = handler;
}

static void _CXwaylandSurfaceV1SetSerial(wl_client* client, wl_resource* resource, uint32_t serial_lo, uint32_t serial_hi) {
    const auto PO = (CXwaylandSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSerial)
        PO->requests.setSerial(PO, serial_lo, serial_hi);
}

static void _CXwaylandSurfaceV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXwaylandSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXwaylandSurfaceV1__DestroyListener(wl_listener* l, void* d) {
    CXwaylandSurfaceV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXwaylandSurfaceV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXwaylandSurfaceV1VTable[] = {
    (void*)_CXwaylandSurfaceV1SetSerial,
    (void*)_CXwaylandSurfaceV1Destroy,
};
static const wl_interface* _CXwaylandSurfaceV1SetSerialTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CXwaylandSurfaceV1Requests[] = {
    { "set_serial", "uu", _CXwaylandSurfaceV1SetSerialTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface xwayland_surface_v1_interface = {
    "xwayland_surface_v1", 1,
    2, _CXwaylandSurfaceV1Requests,
    0, nullptr,
};

CXwaylandSurfaceV1::CXwaylandSurfaceV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xwayland_surface_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXwaylandSurfaceV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXwaylandSurfaceV1VTable, this, nullptr);
}

CXwaylandSurfaceV1::~CXwaylandSurfaceV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXwaylandSurfaceV1::onDestroyCalled() {
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

void CXwaylandSurfaceV1::setSetSerial(F<void(CXwaylandSurfaceV1*, uint32_t, uint32_t)> handler) {
    requests.setSerial = handler;
}

void CXwaylandSurfaceV1::setDestroy(F<void(CXwaylandSurfaceV1*)> handler) {
    requests.destroy = handler;
}

#undef F
