// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// alpha_modifier_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2024 Xaver Hugl

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
#include "alpha-modifier-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wp_alpha_modifier_v1_interface;
extern const wl_interface wp_alpha_modifier_surface_v1_interface;
extern const wl_interface wl_surface_interface;

static void _CWpAlphaModifierV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpAlphaModifierV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpAlphaModifierV1GetSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CWpAlphaModifierV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getSurface)
        PO->requests.getSurface(PO, id, surface);
}

static void _CWpAlphaModifierV1__DestroyListener(wl_listener* l, void* d) {
    CWpAlphaModifierV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpAlphaModifierV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpAlphaModifierV1VTable[] = {
    (void*)_CWpAlphaModifierV1Destroy,
    (void*)_CWpAlphaModifierV1GetSurface,
};
static const wl_interface* _CWpAlphaModifierV1GetSurfaceTypes[] = {
    &wp_alpha_modifier_surface_v1_interface,
    &wl_surface_interface,
};

static const wl_message _CWpAlphaModifierV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_surface", "no", _CWpAlphaModifierV1GetSurfaceTypes + 0},
};

const wl_interface wp_alpha_modifier_v1_interface = {
    "wp_alpha_modifier_v1", 1,
    2, _CWpAlphaModifierV1Requests,
    0, nullptr,
};

CWpAlphaModifierV1::CWpAlphaModifierV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_alpha_modifier_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpAlphaModifierV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpAlphaModifierV1VTable, this, nullptr);
}

CWpAlphaModifierV1::~CWpAlphaModifierV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpAlphaModifierV1::onDestroyCalled() {
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

void CWpAlphaModifierV1::setDestroy(F<void(CWpAlphaModifierV1*)> handler) {
    requests.destroy = handler;
}

void CWpAlphaModifierV1::setGetSurface(F<void(CWpAlphaModifierV1*, uint32_t, wl_resource*)> handler) {
    requests.getSurface = handler;
}

static void _CWpAlphaModifierSurfaceV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpAlphaModifierSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpAlphaModifierSurfaceV1SetMultiplier(wl_client* client, wl_resource* resource, uint32_t factor) {
    const auto PO = (CWpAlphaModifierSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMultiplier)
        PO->requests.setMultiplier(PO, factor);
}

static void _CWpAlphaModifierSurfaceV1__DestroyListener(wl_listener* l, void* d) {
    CWpAlphaModifierSurfaceV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpAlphaModifierSurfaceV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpAlphaModifierSurfaceV1VTable[] = {
    (void*)_CWpAlphaModifierSurfaceV1Destroy,
    (void*)_CWpAlphaModifierSurfaceV1SetMultiplier,
};
static const wl_interface* _CWpAlphaModifierSurfaceV1SetMultiplierTypes[] = {
    nullptr,
};

static const wl_message _CWpAlphaModifierSurfaceV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_multiplier", "u", _CWpAlphaModifierSurfaceV1SetMultiplierTypes + 0},
};

const wl_interface wp_alpha_modifier_surface_v1_interface = {
    "wp_alpha_modifier_surface_v1", 1,
    2, _CWpAlphaModifierSurfaceV1Requests,
    0, nullptr,
};

CWpAlphaModifierSurfaceV1::CWpAlphaModifierSurfaceV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_alpha_modifier_surface_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpAlphaModifierSurfaceV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpAlphaModifierSurfaceV1VTable, this, nullptr);
}

CWpAlphaModifierSurfaceV1::~CWpAlphaModifierSurfaceV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpAlphaModifierSurfaceV1::onDestroyCalled() {
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

void CWpAlphaModifierSurfaceV1::setDestroy(F<void(CWpAlphaModifierSurfaceV1*)> handler) {
    requests.destroy = handler;
}

void CWpAlphaModifierSurfaceV1::setSetMultiplier(F<void(CWpAlphaModifierSurfaceV1*, uint32_t)> handler) {
    requests.setMultiplier = handler;
}

#undef F
