// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// fractional_scale_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2022 Kenny Levinsen

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
#include "fractional-scale-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wp_fractional_scale_manager_v1_interface;
extern const wl_interface wp_fractional_scale_v1_interface;
extern const wl_interface wl_surface_interface;

static void _CWpFractionalScaleManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpFractionalScaleManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpFractionalScaleManagerV1GetFractionalScale(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CWpFractionalScaleManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getFractionalScale)
        PO->requests.getFractionalScale(PO, id, surface);
}

static void _CWpFractionalScaleManagerV1__DestroyListener(wl_listener* l, void* d) {
    CWpFractionalScaleManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpFractionalScaleManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpFractionalScaleManagerV1VTable[] = {
    (void*)_CWpFractionalScaleManagerV1Destroy,
    (void*)_CWpFractionalScaleManagerV1GetFractionalScale,
};
static const wl_interface* _CWpFractionalScaleManagerV1GetFractionalScaleTypes[] = {
    &wp_fractional_scale_v1_interface,
    &wl_surface_interface,
};

static const wl_message _CWpFractionalScaleManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_fractional_scale", "no", _CWpFractionalScaleManagerV1GetFractionalScaleTypes + 0},
};

const wl_interface wp_fractional_scale_manager_v1_interface = {
    "wp_fractional_scale_manager_v1", 1,
    2, _CWpFractionalScaleManagerV1Requests,
    0, nullptr,
};

CWpFractionalScaleManagerV1::CWpFractionalScaleManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_fractional_scale_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpFractionalScaleManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpFractionalScaleManagerV1VTable, this, nullptr);
}

CWpFractionalScaleManagerV1::~CWpFractionalScaleManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpFractionalScaleManagerV1::onDestroyCalled() {
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

void CWpFractionalScaleManagerV1::setDestroy(F<void(CWpFractionalScaleManagerV1*)> handler) {
    requests.destroy = handler;
}

void CWpFractionalScaleManagerV1::setGetFractionalScale(F<void(CWpFractionalScaleManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getFractionalScale = handler;
}

static void _CWpFractionalScaleV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpFractionalScaleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpFractionalScaleV1__DestroyListener(wl_listener* l, void* d) {
    CWpFractionalScaleV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpFractionalScaleV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpFractionalScaleV1VTable[] = {
    (void*)_CWpFractionalScaleV1Destroy,
};

void CWpFractionalScaleV1::sendPreferredScale(uint32_t scale) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, scale);
}

void CWpFractionalScaleV1::sendPreferredScaleRaw(uint32_t scale) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, scale);
}
static const wl_interface* _CWpFractionalScaleV1PreferredScaleTypes[] = {
    nullptr,
};

static const wl_message _CWpFractionalScaleV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CWpFractionalScaleV1Events[] = {
    { "preferred_scale", "u", _CWpFractionalScaleV1PreferredScaleTypes + 0},
};

const wl_interface wp_fractional_scale_v1_interface = {
    "wp_fractional_scale_v1", 1,
    1, _CWpFractionalScaleV1Requests,
    1, _CWpFractionalScaleV1Events,
};

CWpFractionalScaleV1::CWpFractionalScaleV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_fractional_scale_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpFractionalScaleV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpFractionalScaleV1VTable, this, nullptr);
}

CWpFractionalScaleV1::~CWpFractionalScaleV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpFractionalScaleV1::onDestroyCalled() {
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

void CWpFractionalScaleV1::setDestroy(F<void(CWpFractionalScaleV1*)> handler) {
    requests.destroy = handler;
}

#undef F
