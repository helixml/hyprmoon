// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// idle_inhibit_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2015 Samsung Electronics Co., Ltd

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
#include "idle-inhibit-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_idle_inhibit_manager_v1_interface;
extern const wl_interface zwp_idle_inhibitor_v1_interface;
extern const wl_interface wl_surface_interface;

static void _CZwpIdleInhibitManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpIdleInhibitManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpIdleInhibitManagerV1CreateInhibitor(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CZwpIdleInhibitManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createInhibitor)
        PO->requests.createInhibitor(PO, id, surface);
}

static void _CZwpIdleInhibitManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwpIdleInhibitManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpIdleInhibitManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpIdleInhibitManagerV1VTable[] = {
    (void*)_CZwpIdleInhibitManagerV1Destroy,
    (void*)_CZwpIdleInhibitManagerV1CreateInhibitor,
};
static const wl_interface* _CZwpIdleInhibitManagerV1CreateInhibitorTypes[] = {
    &zwp_idle_inhibitor_v1_interface,
    &wl_surface_interface,
};

static const wl_message _CZwpIdleInhibitManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "create_inhibitor", "no", _CZwpIdleInhibitManagerV1CreateInhibitorTypes + 0},
};

const wl_interface zwp_idle_inhibit_manager_v1_interface = {
    "zwp_idle_inhibit_manager_v1", 1,
    2, _CZwpIdleInhibitManagerV1Requests,
    0, nullptr,
};

CZwpIdleInhibitManagerV1::CZwpIdleInhibitManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_idle_inhibit_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpIdleInhibitManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpIdleInhibitManagerV1VTable, this, nullptr);
}

CZwpIdleInhibitManagerV1::~CZwpIdleInhibitManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpIdleInhibitManagerV1::onDestroyCalled() {
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

void CZwpIdleInhibitManagerV1::setDestroy(F<void(CZwpIdleInhibitManagerV1*)> handler) {
    requests.destroy = handler;
}

void CZwpIdleInhibitManagerV1::setCreateInhibitor(F<void(CZwpIdleInhibitManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.createInhibitor = handler;
}

static void _CZwpIdleInhibitorV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpIdleInhibitorV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpIdleInhibitorV1__DestroyListener(wl_listener* l, void* d) {
    CZwpIdleInhibitorV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpIdleInhibitorV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpIdleInhibitorV1VTable[] = {
    (void*)_CZwpIdleInhibitorV1Destroy,
};

static const wl_message _CZwpIdleInhibitorV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

const wl_interface zwp_idle_inhibitor_v1_interface = {
    "zwp_idle_inhibitor_v1", 1,
    1, _CZwpIdleInhibitorV1Requests,
    0, nullptr,
};

CZwpIdleInhibitorV1::CZwpIdleInhibitorV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_idle_inhibitor_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpIdleInhibitorV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpIdleInhibitorV1VTable, this, nullptr);
}

CZwpIdleInhibitorV1::~CZwpIdleInhibitorV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpIdleInhibitorV1::onDestroyCalled() {
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

void CZwpIdleInhibitorV1::setDestroy(F<void(CZwpIdleInhibitorV1*)> handler) {
    requests.destroy = handler;
}

#undef F
