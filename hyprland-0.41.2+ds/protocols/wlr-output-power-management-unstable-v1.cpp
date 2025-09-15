// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wlr_output_power_management_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2019 Purism SPC

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
#include "wlr-output-power-management-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwlr_output_power_manager_v1_interface;
extern const wl_interface zwlr_output_power_v1_interface;
extern const wl_interface wl_output_interface;

static void _CZwlrOutputPowerManagerV1GetOutputPower(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* output) {
    const auto PO = (CZwlrOutputPowerManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getOutputPower)
        PO->requests.getOutputPower(PO, id, output);
}

static void _CZwlrOutputPowerManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputPowerManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrOutputPowerManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputPowerManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputPowerManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputPowerManagerV1VTable[] = {
    (void*)_CZwlrOutputPowerManagerV1GetOutputPower,
    (void*)_CZwlrOutputPowerManagerV1Destroy,
};
static const wl_interface* _CZwlrOutputPowerManagerV1GetOutputPowerTypes[] = {
    &zwlr_output_power_v1_interface,
    &wl_output_interface,
};

static const wl_message _CZwlrOutputPowerManagerV1Requests[] = {
    { "get_output_power", "no", _CZwlrOutputPowerManagerV1GetOutputPowerTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface zwlr_output_power_manager_v1_interface = {
    "zwlr_output_power_manager_v1", 1,
    2, _CZwlrOutputPowerManagerV1Requests,
    0, nullptr,
};

CZwlrOutputPowerManagerV1::CZwlrOutputPowerManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_power_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputPowerManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputPowerManagerV1VTable, this, nullptr);
}

CZwlrOutputPowerManagerV1::~CZwlrOutputPowerManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputPowerManagerV1::onDestroyCalled() {
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

void CZwlrOutputPowerManagerV1::setGetOutputPower(F<void(CZwlrOutputPowerManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getOutputPower = handler;
}

void CZwlrOutputPowerManagerV1::setDestroy(F<void(CZwlrOutputPowerManagerV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwlrOutputPowerV1SetMode(wl_client* client, wl_resource* resource, zwlrOutputPowerV1Mode mode) {
    const auto PO = (CZwlrOutputPowerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMode)
        PO->requests.setMode(PO, mode);
}

static void _CZwlrOutputPowerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputPowerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrOutputPowerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputPowerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputPowerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputPowerV1VTable[] = {
    (void*)_CZwlrOutputPowerV1SetMode,
    (void*)_CZwlrOutputPowerV1Destroy,
};

void CZwlrOutputPowerV1::sendMode(zwlrOutputPowerV1Mode mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}

void CZwlrOutputPowerV1::sendFailed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwlrOutputPowerV1::sendModeRaw(zwlrOutputPowerV1Mode mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}

void CZwlrOutputPowerV1::sendFailedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}
static const wl_interface* _CZwlrOutputPowerV1SetModeTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputPowerV1ModeTypes[] = {
    nullptr,
};

static const wl_message _CZwlrOutputPowerV1Requests[] = {
    { "set_mode", "u", _CZwlrOutputPowerV1SetModeTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwlrOutputPowerV1Events[] = {
    { "mode", "u", _CZwlrOutputPowerV1ModeTypes + 0},
    { "failed", "", dummyTypes + 0},
};

const wl_interface zwlr_output_power_v1_interface = {
    "zwlr_output_power_v1", 1,
    2, _CZwlrOutputPowerV1Requests,
    2, _CZwlrOutputPowerV1Events,
};

CZwlrOutputPowerV1::CZwlrOutputPowerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_power_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputPowerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputPowerV1VTable, this, nullptr);
}

CZwlrOutputPowerV1::~CZwlrOutputPowerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputPowerV1::onDestroyCalled() {
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

void CZwlrOutputPowerV1::setSetMode(F<void(CZwlrOutputPowerV1*, zwlrOutputPowerV1Mode)> handler) {
    requests.setMode = handler;
}

void CZwlrOutputPowerV1::setDestroy(F<void(CZwlrOutputPowerV1*)> handler) {
    requests.destroy = handler;
}

#undef F
