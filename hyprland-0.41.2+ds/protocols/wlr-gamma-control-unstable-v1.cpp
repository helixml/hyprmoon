// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wlr_gamma_control_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2015 Giulio camuffo
    Copyright © 2018 Simon Ser

    Permission to use, copy, modify, distribute, and sell this
    software and its documentation for any purpose is hereby granted
    without fee, provided that the above copyright notice appear in
    all copies and that both that copyright notice and this permission
    notice appear in supporting documentation, and that the name of
    the copyright holders not be used in advertising or publicity
    pertaining to distribution of the software without specific,
    written prior permission.  The copyright holders make no
    representations about the suitability of this software for any
    purpose.  It is provided "as is" without express or implied
    warranty.

    THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
    SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
    SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
    AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
    ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
    THIS SOFTWARE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "wlr-gamma-control-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwlr_gamma_control_manager_v1_interface;
extern const wl_interface zwlr_gamma_control_v1_interface;
extern const wl_interface wl_output_interface;

static void _CZwlrGammaControlManagerV1GetGammaControl(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* output) {
    const auto PO = (CZwlrGammaControlManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getGammaControl)
        PO->requests.getGammaControl(PO, id, output);
}

static void _CZwlrGammaControlManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrGammaControlManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrGammaControlManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrGammaControlManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrGammaControlManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrGammaControlManagerV1VTable[] = {
    (void*)_CZwlrGammaControlManagerV1GetGammaControl,
    (void*)_CZwlrGammaControlManagerV1Destroy,
};
static const wl_interface* _CZwlrGammaControlManagerV1GetGammaControlTypes[] = {
    &zwlr_gamma_control_v1_interface,
    &wl_output_interface,
};

static const wl_message _CZwlrGammaControlManagerV1Requests[] = {
    { "get_gamma_control", "no", _CZwlrGammaControlManagerV1GetGammaControlTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface zwlr_gamma_control_manager_v1_interface = {
    "zwlr_gamma_control_manager_v1", 1,
    2, _CZwlrGammaControlManagerV1Requests,
    0, nullptr,
};

CZwlrGammaControlManagerV1::CZwlrGammaControlManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_gamma_control_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrGammaControlManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrGammaControlManagerV1VTable, this, nullptr);
}

CZwlrGammaControlManagerV1::~CZwlrGammaControlManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrGammaControlManagerV1::onDestroyCalled() {
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

void CZwlrGammaControlManagerV1::setGetGammaControl(F<void(CZwlrGammaControlManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getGammaControl = handler;
}

void CZwlrGammaControlManagerV1::setDestroy(F<void(CZwlrGammaControlManagerV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwlrGammaControlV1SetGamma(wl_client* client, wl_resource* resource, int32_t fd) {
    const auto PO = (CZwlrGammaControlV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setGamma)
        PO->requests.setGamma(PO, fd);
}

static void _CZwlrGammaControlV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrGammaControlV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrGammaControlV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrGammaControlV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrGammaControlV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrGammaControlV1VTable[] = {
    (void*)_CZwlrGammaControlV1SetGamma,
    (void*)_CZwlrGammaControlV1Destroy,
};

void CZwlrGammaControlV1::sendGammaSize(uint32_t size) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, size);
}

void CZwlrGammaControlV1::sendFailed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwlrGammaControlV1::sendGammaSizeRaw(uint32_t size) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, size);
}

void CZwlrGammaControlV1::sendFailedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}
static const wl_interface* _CZwlrGammaControlV1SetGammaTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrGammaControlV1GammaSizeTypes[] = {
    nullptr,
};

static const wl_message _CZwlrGammaControlV1Requests[] = {
    { "set_gamma", "h", _CZwlrGammaControlV1SetGammaTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwlrGammaControlV1Events[] = {
    { "gamma_size", "u", _CZwlrGammaControlV1GammaSizeTypes + 0},
    { "failed", "", dummyTypes + 0},
};

const wl_interface zwlr_gamma_control_v1_interface = {
    "zwlr_gamma_control_v1", 1,
    2, _CZwlrGammaControlV1Requests,
    2, _CZwlrGammaControlV1Events,
};

CZwlrGammaControlV1::CZwlrGammaControlV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_gamma_control_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrGammaControlV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrGammaControlV1VTable, this, nullptr);
}

CZwlrGammaControlV1::~CZwlrGammaControlV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrGammaControlV1::onDestroyCalled() {
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

void CZwlrGammaControlV1::setSetGamma(F<void(CZwlrGammaControlV1*, int32_t)> handler) {
    requests.setGamma = handler;
}

void CZwlrGammaControlV1::setDestroy(F<void(CZwlrGammaControlV1*)> handler) {
    requests.destroy = handler;
}

#undef F
