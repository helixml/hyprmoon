// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wlr_layer_shell_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2017 Drew DeVault

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
#include "wlr-layer-shell-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwlr_layer_shell_v1_interface;
extern const wl_interface zwlr_layer_surface_v1_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_output_interface;
extern const wl_interface xdg_popup_interface;

static void _CZwlrLayerShellV1GetLayerSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface, wl_resource* output, zwlrLayerShellV1Layer layer, const char* namespace_) {
    const auto PO = (CZwlrLayerShellV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getLayerSurface)
        PO->requests.getLayerSurface(PO, id, surface, output, layer, namespace_);
}

static void _CZwlrLayerShellV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrLayerShellV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrLayerShellV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrLayerShellV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrLayerShellV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrLayerShellV1VTable[] = {
    (void*)_CZwlrLayerShellV1GetLayerSurface,
    (void*)_CZwlrLayerShellV1Destroy,
};
static const wl_interface* _CZwlrLayerShellV1GetLayerSurfaceTypes[] = {
    &zwlr_layer_surface_v1_interface,
    &wl_surface_interface,
    &wl_output_interface,
    nullptr,
    nullptr,
};

static const wl_message _CZwlrLayerShellV1Requests[] = {
    { "get_layer_surface", "no?ous", _CZwlrLayerShellV1GetLayerSurfaceTypes + 0},
    { "destroy", "3", dummyTypes + 0},
};

const wl_interface zwlr_layer_shell_v1_interface = {
    "zwlr_layer_shell_v1", 5,
    2, _CZwlrLayerShellV1Requests,
    0, nullptr,
};

CZwlrLayerShellV1::CZwlrLayerShellV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_layer_shell_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrLayerShellV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrLayerShellV1VTable, this, nullptr);
}

CZwlrLayerShellV1::~CZwlrLayerShellV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrLayerShellV1::onDestroyCalled() {
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

void CZwlrLayerShellV1::setGetLayerSurface(F<void(CZwlrLayerShellV1*, uint32_t, wl_resource*, wl_resource*, zwlrLayerShellV1Layer, const char*)> handler) {
    requests.getLayerSurface = handler;
}

void CZwlrLayerShellV1::setDestroy(F<void(CZwlrLayerShellV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwlrLayerSurfaceV1SetSize(wl_client* client, wl_resource* resource, uint32_t width, uint32_t height) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSize)
        PO->requests.setSize(PO, width, height);
}

static void _CZwlrLayerSurfaceV1SetAnchor(wl_client* client, wl_resource* resource, zwlrLayerSurfaceV1Anchor anchor) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setAnchor)
        PO->requests.setAnchor(PO, anchor);
}

static void _CZwlrLayerSurfaceV1SetExclusiveZone(wl_client* client, wl_resource* resource, int32_t zone) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setExclusiveZone)
        PO->requests.setExclusiveZone(PO, zone);
}

static void _CZwlrLayerSurfaceV1SetMargin(wl_client* client, wl_resource* resource, int32_t top, int32_t right, int32_t bottom, int32_t left) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMargin)
        PO->requests.setMargin(PO, top, right, bottom, left);
}

static void _CZwlrLayerSurfaceV1SetKeyboardInteractivity(wl_client* client, wl_resource* resource, zwlrLayerSurfaceV1KeyboardInteractivity keyboard_interactivity) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setKeyboardInteractivity)
        PO->requests.setKeyboardInteractivity(PO, keyboard_interactivity);
}

static void _CZwlrLayerSurfaceV1GetPopup(wl_client* client, wl_resource* resource, wl_resource* popup) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getPopup)
        PO->requests.getPopup(PO, popup);
}

static void _CZwlrLayerSurfaceV1AckConfigure(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.ackConfigure)
        PO->requests.ackConfigure(PO, serial);
}

static void _CZwlrLayerSurfaceV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrLayerSurfaceV1SetLayer(wl_client* client, wl_resource* resource, uint32_t layer) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setLayer)
        PO->requests.setLayer(PO, layer);
}

static void _CZwlrLayerSurfaceV1SetExclusiveEdge(wl_client* client, wl_resource* resource, zwlrLayerSurfaceV1Anchor edge) {
    const auto PO = (CZwlrLayerSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setExclusiveEdge)
        PO->requests.setExclusiveEdge(PO, edge);
}

static void _CZwlrLayerSurfaceV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrLayerSurfaceV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrLayerSurfaceV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrLayerSurfaceV1VTable[] = {
    (void*)_CZwlrLayerSurfaceV1SetSize,
    (void*)_CZwlrLayerSurfaceV1SetAnchor,
    (void*)_CZwlrLayerSurfaceV1SetExclusiveZone,
    (void*)_CZwlrLayerSurfaceV1SetMargin,
    (void*)_CZwlrLayerSurfaceV1SetKeyboardInteractivity,
    (void*)_CZwlrLayerSurfaceV1GetPopup,
    (void*)_CZwlrLayerSurfaceV1AckConfigure,
    (void*)_CZwlrLayerSurfaceV1Destroy,
    (void*)_CZwlrLayerSurfaceV1SetLayer,
    (void*)_CZwlrLayerSurfaceV1SetExclusiveEdge,
};

void CZwlrLayerSurfaceV1::sendConfigure(uint32_t serial, uint32_t width, uint32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, width, height);
}

void CZwlrLayerSurfaceV1::sendClosed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwlrLayerSurfaceV1::sendConfigureRaw(uint32_t serial, uint32_t width, uint32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, width, height);
}

void CZwlrLayerSurfaceV1::sendClosedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}
static const wl_interface* _CZwlrLayerSurfaceV1SetSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1SetAnchorTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1SetExclusiveZoneTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1SetMarginTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1SetKeyboardInteractivityTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1GetPopupTypes[] = {
    &xdg_popup_interface,
};
static const wl_interface* _CZwlrLayerSurfaceV1AckConfigureTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1SetLayerTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1SetExclusiveEdgeTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrLayerSurfaceV1ConfigureTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwlrLayerSurfaceV1Requests[] = {
    { "set_size", "uu", _CZwlrLayerSurfaceV1SetSizeTypes + 0},
    { "set_anchor", "u", _CZwlrLayerSurfaceV1SetAnchorTypes + 0},
    { "set_exclusive_zone", "i", _CZwlrLayerSurfaceV1SetExclusiveZoneTypes + 0},
    { "set_margin", "iiii", _CZwlrLayerSurfaceV1SetMarginTypes + 0},
    { "set_keyboard_interactivity", "u", _CZwlrLayerSurfaceV1SetKeyboardInteractivityTypes + 0},
    { "get_popup", "o", _CZwlrLayerSurfaceV1GetPopupTypes + 0},
    { "ack_configure", "u", _CZwlrLayerSurfaceV1AckConfigureTypes + 0},
    { "destroy", "", dummyTypes + 0},
    { "set_layer", "2u", _CZwlrLayerSurfaceV1SetLayerTypes + 0},
    { "set_exclusive_edge", "5u", _CZwlrLayerSurfaceV1SetExclusiveEdgeTypes + 0},
};

static const wl_message _CZwlrLayerSurfaceV1Events[] = {
    { "configure", "uuu", _CZwlrLayerSurfaceV1ConfigureTypes + 0},
    { "closed", "", dummyTypes + 0},
};

const wl_interface zwlr_layer_surface_v1_interface = {
    "zwlr_layer_surface_v1", 5,
    10, _CZwlrLayerSurfaceV1Requests,
    2, _CZwlrLayerSurfaceV1Events,
};

CZwlrLayerSurfaceV1::CZwlrLayerSurfaceV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_layer_surface_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrLayerSurfaceV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrLayerSurfaceV1VTable, this, nullptr);
}

CZwlrLayerSurfaceV1::~CZwlrLayerSurfaceV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrLayerSurfaceV1::onDestroyCalled() {
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

void CZwlrLayerSurfaceV1::setSetSize(F<void(CZwlrLayerSurfaceV1*, uint32_t, uint32_t)> handler) {
    requests.setSize = handler;
}

void CZwlrLayerSurfaceV1::setSetAnchor(F<void(CZwlrLayerSurfaceV1*, zwlrLayerSurfaceV1Anchor)> handler) {
    requests.setAnchor = handler;
}

void CZwlrLayerSurfaceV1::setSetExclusiveZone(F<void(CZwlrLayerSurfaceV1*, int32_t)> handler) {
    requests.setExclusiveZone = handler;
}

void CZwlrLayerSurfaceV1::setSetMargin(F<void(CZwlrLayerSurfaceV1*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.setMargin = handler;
}

void CZwlrLayerSurfaceV1::setSetKeyboardInteractivity(F<void(CZwlrLayerSurfaceV1*, zwlrLayerSurfaceV1KeyboardInteractivity)> handler) {
    requests.setKeyboardInteractivity = handler;
}

void CZwlrLayerSurfaceV1::setGetPopup(F<void(CZwlrLayerSurfaceV1*, wl_resource*)> handler) {
    requests.getPopup = handler;
}

void CZwlrLayerSurfaceV1::setAckConfigure(F<void(CZwlrLayerSurfaceV1*, uint32_t)> handler) {
    requests.ackConfigure = handler;
}

void CZwlrLayerSurfaceV1::setDestroy(F<void(CZwlrLayerSurfaceV1*)> handler) {
    requests.destroy = handler;
}

void CZwlrLayerSurfaceV1::setSetLayer(F<void(CZwlrLayerSurfaceV1*, uint32_t)> handler) {
    requests.setLayer = handler;
}

void CZwlrLayerSurfaceV1::setSetExclusiveEdge(F<void(CZwlrLayerSurfaceV1*, zwlrLayerSurfaceV1Anchor)> handler) {
    requests.setExclusiveEdge = handler;
}

#undef F
