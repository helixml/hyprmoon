// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// tearing_control_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2021 Xaver Hugl

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
#include "tearing-control-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wp_tearing_control_manager_v1_interface;
extern const wl_interface wp_tearing_control_v1_interface;
extern const wl_interface wl_surface_interface;

static void _CWpTearingControlManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpTearingControlManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpTearingControlManagerV1GetTearingControl(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CWpTearingControlManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getTearingControl)
        PO->requests.getTearingControl(PO, id, surface);
}

static void _CWpTearingControlManagerV1__DestroyListener(wl_listener* l, void* d) {
    CWpTearingControlManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpTearingControlManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpTearingControlManagerV1VTable[] = {
    (void*)_CWpTearingControlManagerV1Destroy,
    (void*)_CWpTearingControlManagerV1GetTearingControl,
};
static const wl_interface* _CWpTearingControlManagerV1GetTearingControlTypes[] = {
    &wp_tearing_control_v1_interface,
    &wl_surface_interface,
};

static const wl_message _CWpTearingControlManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_tearing_control", "no", _CWpTearingControlManagerV1GetTearingControlTypes + 0},
};

const wl_interface wp_tearing_control_manager_v1_interface = {
    "wp_tearing_control_manager_v1", 1,
    2, _CWpTearingControlManagerV1Requests,
    0, nullptr,
};

CWpTearingControlManagerV1::CWpTearingControlManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_tearing_control_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpTearingControlManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpTearingControlManagerV1VTable, this, nullptr);
}

CWpTearingControlManagerV1::~CWpTearingControlManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpTearingControlManagerV1::onDestroyCalled() {
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

void CWpTearingControlManagerV1::setDestroy(F<void(CWpTearingControlManagerV1*)> handler) {
    requests.destroy = handler;
}

void CWpTearingControlManagerV1::setGetTearingControl(F<void(CWpTearingControlManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getTearingControl = handler;
}

static void _CWpTearingControlV1SetPresentationHint(wl_client* client, wl_resource* resource, wpTearingControlV1PresentationHint hint) {
    const auto PO = (CWpTearingControlV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setPresentationHint)
        PO->requests.setPresentationHint(PO, hint);
}

static void _CWpTearingControlV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpTearingControlV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpTearingControlV1__DestroyListener(wl_listener* l, void* d) {
    CWpTearingControlV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpTearingControlV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpTearingControlV1VTable[] = {
    (void*)_CWpTearingControlV1SetPresentationHint,
    (void*)_CWpTearingControlV1Destroy,
};
static const wl_interface* _CWpTearingControlV1SetPresentationHintTypes[] = {
    nullptr,
};

static const wl_message _CWpTearingControlV1Requests[] = {
    { "set_presentation_hint", "u", _CWpTearingControlV1SetPresentationHintTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface wp_tearing_control_v1_interface = {
    "wp_tearing_control_v1", 1,
    2, _CWpTearingControlV1Requests,
    0, nullptr,
};

CWpTearingControlV1::CWpTearingControlV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_tearing_control_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpTearingControlV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpTearingControlV1VTable, this, nullptr);
}

CWpTearingControlV1::~CWpTearingControlV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpTearingControlV1::onDestroyCalled() {
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

void CWpTearingControlV1::setSetPresentationHint(F<void(CWpTearingControlV1*, wpTearingControlV1PresentationHint)> handler) {
    requests.setPresentationHint = handler;
}

void CWpTearingControlV1::setDestroy(F<void(CWpTearingControlV1*)> handler) {
    requests.destroy = handler;
}

#undef F
