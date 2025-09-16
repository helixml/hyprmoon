// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// xdg_decoration_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2018 Simon Ser

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
#include "xdg-decoration-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zxdg_decoration_manager_v1_interface;
extern const wl_interface zxdg_toplevel_decoration_v1_interface;
extern const wl_interface xdg_toplevel_interface;

static void _CZxdgDecorationManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZxdgDecorationManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZxdgDecorationManagerV1GetToplevelDecoration(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* toplevel) {
    const auto PO = (CZxdgDecorationManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getToplevelDecoration)
        PO->requests.getToplevelDecoration(PO, id, toplevel);
}

static void _CZxdgDecorationManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZxdgDecorationManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZxdgDecorationManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZxdgDecorationManagerV1VTable[] = {
    (void*)_CZxdgDecorationManagerV1Destroy,
    (void*)_CZxdgDecorationManagerV1GetToplevelDecoration,
};
static const wl_interface* _CZxdgDecorationManagerV1GetToplevelDecorationTypes[] = {
    &zxdg_toplevel_decoration_v1_interface,
    &xdg_toplevel_interface,
};

static const wl_message _CZxdgDecorationManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_toplevel_decoration", "no", _CZxdgDecorationManagerV1GetToplevelDecorationTypes + 0},
};

const wl_interface zxdg_decoration_manager_v1_interface = {
    "zxdg_decoration_manager_v1", 1,
    2, _CZxdgDecorationManagerV1Requests,
    0, nullptr,
};

CZxdgDecorationManagerV1::CZxdgDecorationManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zxdg_decoration_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZxdgDecorationManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZxdgDecorationManagerV1VTable, this, nullptr);
}

CZxdgDecorationManagerV1::~CZxdgDecorationManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZxdgDecorationManagerV1::onDestroyCalled() {
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

void CZxdgDecorationManagerV1::setDestroy(F<void(CZxdgDecorationManagerV1*)> handler) {
    requests.destroy = handler;
}

void CZxdgDecorationManagerV1::setGetToplevelDecoration(F<void(CZxdgDecorationManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getToplevelDecoration = handler;
}

static void _CZxdgToplevelDecorationV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZxdgToplevelDecorationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZxdgToplevelDecorationV1SetMode(wl_client* client, wl_resource* resource, zxdgToplevelDecorationV1Mode mode) {
    const auto PO = (CZxdgToplevelDecorationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMode)
        PO->requests.setMode(PO, mode);
}

static void _CZxdgToplevelDecorationV1UnsetMode(wl_client* client, wl_resource* resource) {
    const auto PO = (CZxdgToplevelDecorationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unsetMode)
        PO->requests.unsetMode(PO);
}

static void _CZxdgToplevelDecorationV1__DestroyListener(wl_listener* l, void* d) {
    CZxdgToplevelDecorationV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZxdgToplevelDecorationV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZxdgToplevelDecorationV1VTable[] = {
    (void*)_CZxdgToplevelDecorationV1Destroy,
    (void*)_CZxdgToplevelDecorationV1SetMode,
    (void*)_CZxdgToplevelDecorationV1UnsetMode,
};

void CZxdgToplevelDecorationV1::sendConfigure(zxdgToplevelDecorationV1Mode mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}

void CZxdgToplevelDecorationV1::sendConfigureRaw(zxdgToplevelDecorationV1Mode mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}
static const wl_interface* _CZxdgToplevelDecorationV1SetModeTypes[] = {
    nullptr,
};
static const wl_interface* _CZxdgToplevelDecorationV1ConfigureTypes[] = {
    nullptr,
};

static const wl_message _CZxdgToplevelDecorationV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_mode", "u", _CZxdgToplevelDecorationV1SetModeTypes + 0},
    { "unset_mode", "", dummyTypes + 0},
};

static const wl_message _CZxdgToplevelDecorationV1Events[] = {
    { "configure", "u", _CZxdgToplevelDecorationV1ConfigureTypes + 0},
};

const wl_interface zxdg_toplevel_decoration_v1_interface = {
    "zxdg_toplevel_decoration_v1", 1,
    3, _CZxdgToplevelDecorationV1Requests,
    1, _CZxdgToplevelDecorationV1Events,
};

CZxdgToplevelDecorationV1::CZxdgToplevelDecorationV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zxdg_toplevel_decoration_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZxdgToplevelDecorationV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZxdgToplevelDecorationV1VTable, this, nullptr);
}

CZxdgToplevelDecorationV1::~CZxdgToplevelDecorationV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZxdgToplevelDecorationV1::onDestroyCalled() {
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

void CZxdgToplevelDecorationV1::setDestroy(F<void(CZxdgToplevelDecorationV1*)> handler) {
    requests.destroy = handler;
}

void CZxdgToplevelDecorationV1::setSetMode(F<void(CZxdgToplevelDecorationV1*, zxdgToplevelDecorationV1Mode)> handler) {
    requests.setMode = handler;
}

void CZxdgToplevelDecorationV1::setUnsetMode(F<void(CZxdgToplevelDecorationV1*)> handler) {
    requests.unsetMode = handler;
}

#undef F
