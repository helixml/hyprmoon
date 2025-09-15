// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// xdg_shell

/*
 This protocol's authors' copyright notice is:


    Copyright © 2008-2013 Kristian Høgsberg
    Copyright © 2013      Rafael Antognolli
    Copyright © 2013      Jasper St. Pierre
    Copyright © 2010-2013 Intel Corporation
    Copyright © 2015-2017 Samsung Electronics Co., Ltd
    Copyright © 2015-2017 Red Hat Inc.

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
#include "xdg-shell.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface xdg_wm_base_interface;
extern const wl_interface xdg_positioner_interface;
extern const wl_interface xdg_surface_interface;
extern const wl_interface xdg_toplevel_interface;
extern const wl_interface xdg_popup_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_seat_interface;
extern const wl_interface wl_output_interface;

static void _CXdgWmBaseDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgWmBase*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgWmBaseCreatePositioner(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CXdgWmBase*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createPositioner)
        PO->requests.createPositioner(PO, id);
}

static void _CXdgWmBaseGetXdgSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CXdgWmBase*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getXdgSurface)
        PO->requests.getXdgSurface(PO, id, surface);
}

static void _CXdgWmBasePong(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CXdgWmBase*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.pong)
        PO->requests.pong(PO, serial);
}

static void _CXdgWmBase__DestroyListener(wl_listener* l, void* d) {
    CXdgWmBaseDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgWmBase* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgWmBaseVTable[] = {
    (void*)_CXdgWmBaseDestroy,
    (void*)_CXdgWmBaseCreatePositioner,
    (void*)_CXdgWmBaseGetXdgSurface,
    (void*)_CXdgWmBasePong,
};

void CXdgWmBase::sendPing(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial);
}

void CXdgWmBase::sendPingRaw(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial);
}
static const wl_interface* _CXdgWmBaseCreatePositionerTypes[] = {
    &xdg_positioner_interface,
};
static const wl_interface* _CXdgWmBaseGetXdgSurfaceTypes[] = {
    &xdg_surface_interface,
    &wl_surface_interface,
};
static const wl_interface* _CXdgWmBasePongTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgWmBasePingTypes[] = {
    nullptr,
};

static const wl_message _CXdgWmBaseRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "create_positioner", "n", _CXdgWmBaseCreatePositionerTypes + 0},
    { "get_xdg_surface", "no", _CXdgWmBaseGetXdgSurfaceTypes + 0},
    { "pong", "u", _CXdgWmBasePongTypes + 0},
};

static const wl_message _CXdgWmBaseEvents[] = {
    { "ping", "u", _CXdgWmBasePingTypes + 0},
};

const wl_interface xdg_wm_base_interface = {
    "xdg_wm_base", 6,
    4, _CXdgWmBaseRequests,
    1, _CXdgWmBaseEvents,
};

CXdgWmBase::CXdgWmBase(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_wm_base_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgWmBase__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgWmBaseVTable, this, nullptr);
}

CXdgWmBase::~CXdgWmBase() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgWmBase::onDestroyCalled() {
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

void CXdgWmBase::setDestroy(F<void(CXdgWmBase*)> handler) {
    requests.destroy = handler;
}

void CXdgWmBase::setCreatePositioner(F<void(CXdgWmBase*, uint32_t)> handler) {
    requests.createPositioner = handler;
}

void CXdgWmBase::setGetXdgSurface(F<void(CXdgWmBase*, uint32_t, wl_resource*)> handler) {
    requests.getXdgSurface = handler;
}

void CXdgWmBase::setPong(F<void(CXdgWmBase*, uint32_t)> handler) {
    requests.pong = handler;
}

static void _CXdgPositionerDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgPositionerSetSize(wl_client* client, wl_resource* resource, int32_t width, int32_t height) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSize)
        PO->requests.setSize(PO, width, height);
}

static void _CXdgPositionerSetAnchorRect(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setAnchorRect)
        PO->requests.setAnchorRect(PO, x, y, width, height);
}

static void _CXdgPositionerSetAnchor(wl_client* client, wl_resource* resource, xdgPositionerAnchor anchor) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setAnchor)
        PO->requests.setAnchor(PO, anchor);
}

static void _CXdgPositionerSetGravity(wl_client* client, wl_resource* resource, xdgPositionerGravity gravity) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setGravity)
        PO->requests.setGravity(PO, gravity);
}

static void _CXdgPositionerSetConstraintAdjustment(wl_client* client, wl_resource* resource, xdgPositionerConstraintAdjustment constraint_adjustment) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setConstraintAdjustment)
        PO->requests.setConstraintAdjustment(PO, constraint_adjustment);
}

static void _CXdgPositionerSetOffset(wl_client* client, wl_resource* resource, int32_t x, int32_t y) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setOffset)
        PO->requests.setOffset(PO, x, y);
}

static void _CXdgPositionerSetReactive(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setReactive)
        PO->requests.setReactive(PO);
}

static void _CXdgPositionerSetParentSize(wl_client* client, wl_resource* resource, int32_t parent_width, int32_t parent_height) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setParentSize)
        PO->requests.setParentSize(PO, parent_width, parent_height);
}

static void _CXdgPositionerSetParentConfigure(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CXdgPositioner*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setParentConfigure)
        PO->requests.setParentConfigure(PO, serial);
}

static void _CXdgPositioner__DestroyListener(wl_listener* l, void* d) {
    CXdgPositionerDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgPositioner* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgPositionerVTable[] = {
    (void*)_CXdgPositionerDestroy,
    (void*)_CXdgPositionerSetSize,
    (void*)_CXdgPositionerSetAnchorRect,
    (void*)_CXdgPositionerSetAnchor,
    (void*)_CXdgPositionerSetGravity,
    (void*)_CXdgPositionerSetConstraintAdjustment,
    (void*)_CXdgPositionerSetOffset,
    (void*)_CXdgPositionerSetReactive,
    (void*)_CXdgPositionerSetParentSize,
    (void*)_CXdgPositionerSetParentConfigure,
};
static const wl_interface* _CXdgPositionerSetSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgPositionerSetAnchorRectTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgPositionerSetAnchorTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgPositionerSetGravityTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgPositionerSetConstraintAdjustmentTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgPositionerSetOffsetTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgPositionerSetParentSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgPositionerSetParentConfigureTypes[] = {
    nullptr,
};

static const wl_message _CXdgPositionerRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_size", "ii", _CXdgPositionerSetSizeTypes + 0},
    { "set_anchor_rect", "iiii", _CXdgPositionerSetAnchorRectTypes + 0},
    { "set_anchor", "u", _CXdgPositionerSetAnchorTypes + 0},
    { "set_gravity", "u", _CXdgPositionerSetGravityTypes + 0},
    { "set_constraint_adjustment", "u", _CXdgPositionerSetConstraintAdjustmentTypes + 0},
    { "set_offset", "ii", _CXdgPositionerSetOffsetTypes + 0},
    { "set_reactive", "3", dummyTypes + 0},
    { "set_parent_size", "3ii", _CXdgPositionerSetParentSizeTypes + 0},
    { "set_parent_configure", "3u", _CXdgPositionerSetParentConfigureTypes + 0},
};

const wl_interface xdg_positioner_interface = {
    "xdg_positioner", 6,
    10, _CXdgPositionerRequests,
    0, nullptr,
};

CXdgPositioner::CXdgPositioner(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_positioner_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgPositioner__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgPositionerVTable, this, nullptr);
}

CXdgPositioner::~CXdgPositioner() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgPositioner::onDestroyCalled() {
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

void CXdgPositioner::setDestroy(F<void(CXdgPositioner*)> handler) {
    requests.destroy = handler;
}

void CXdgPositioner::setSetSize(F<void(CXdgPositioner*, int32_t, int32_t)> handler) {
    requests.setSize = handler;
}

void CXdgPositioner::setSetAnchorRect(F<void(CXdgPositioner*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.setAnchorRect = handler;
}

void CXdgPositioner::setSetAnchor(F<void(CXdgPositioner*, xdgPositionerAnchor)> handler) {
    requests.setAnchor = handler;
}

void CXdgPositioner::setSetGravity(F<void(CXdgPositioner*, xdgPositionerGravity)> handler) {
    requests.setGravity = handler;
}

void CXdgPositioner::setSetConstraintAdjustment(F<void(CXdgPositioner*, xdgPositionerConstraintAdjustment)> handler) {
    requests.setConstraintAdjustment = handler;
}

void CXdgPositioner::setSetOffset(F<void(CXdgPositioner*, int32_t, int32_t)> handler) {
    requests.setOffset = handler;
}

void CXdgPositioner::setSetReactive(F<void(CXdgPositioner*)> handler) {
    requests.setReactive = handler;
}

void CXdgPositioner::setSetParentSize(F<void(CXdgPositioner*, int32_t, int32_t)> handler) {
    requests.setParentSize = handler;
}

void CXdgPositioner::setSetParentConfigure(F<void(CXdgPositioner*, uint32_t)> handler) {
    requests.setParentConfigure = handler;
}

static void _CXdgSurfaceDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgSurfaceGetToplevel(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CXdgSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getToplevel)
        PO->requests.getToplevel(PO, id);
}

static void _CXdgSurfaceGetPopup(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* parent, wl_resource* positioner) {
    const auto PO = (CXdgSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getPopup)
        PO->requests.getPopup(PO, id, parent, positioner);
}

static void _CXdgSurfaceSetWindowGeometry(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CXdgSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setWindowGeometry)
        PO->requests.setWindowGeometry(PO, x, y, width, height);
}

static void _CXdgSurfaceAckConfigure(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CXdgSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.ackConfigure)
        PO->requests.ackConfigure(PO, serial);
}

static void _CXdgSurface__DestroyListener(wl_listener* l, void* d) {
    CXdgSurfaceDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgSurface* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgSurfaceVTable[] = {
    (void*)_CXdgSurfaceDestroy,
    (void*)_CXdgSurfaceGetToplevel,
    (void*)_CXdgSurfaceGetPopup,
    (void*)_CXdgSurfaceSetWindowGeometry,
    (void*)_CXdgSurfaceAckConfigure,
};

void CXdgSurface::sendConfigure(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial);
}

void CXdgSurface::sendConfigureRaw(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial);
}
static const wl_interface* _CXdgSurfaceGetToplevelTypes[] = {
    &xdg_toplevel_interface,
};
static const wl_interface* _CXdgSurfaceGetPopupTypes[] = {
    &xdg_popup_interface,
    &xdg_surface_interface,
    &xdg_positioner_interface,
};
static const wl_interface* _CXdgSurfaceSetWindowGeometryTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgSurfaceAckConfigureTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgSurfaceConfigureTypes[] = {
    nullptr,
};

static const wl_message _CXdgSurfaceRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_toplevel", "n", _CXdgSurfaceGetToplevelTypes + 0},
    { "get_popup", "n?oo", _CXdgSurfaceGetPopupTypes + 0},
    { "set_window_geometry", "iiii", _CXdgSurfaceSetWindowGeometryTypes + 0},
    { "ack_configure", "u", _CXdgSurfaceAckConfigureTypes + 0},
};

static const wl_message _CXdgSurfaceEvents[] = {
    { "configure", "u", _CXdgSurfaceConfigureTypes + 0},
};

const wl_interface xdg_surface_interface = {
    "xdg_surface", 6,
    5, _CXdgSurfaceRequests,
    1, _CXdgSurfaceEvents,
};

CXdgSurface::CXdgSurface(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_surface_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgSurface__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgSurfaceVTable, this, nullptr);
}

CXdgSurface::~CXdgSurface() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgSurface::onDestroyCalled() {
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

void CXdgSurface::setDestroy(F<void(CXdgSurface*)> handler) {
    requests.destroy = handler;
}

void CXdgSurface::setGetToplevel(F<void(CXdgSurface*, uint32_t)> handler) {
    requests.getToplevel = handler;
}

void CXdgSurface::setGetPopup(F<void(CXdgSurface*, uint32_t, wl_resource*, wl_resource*)> handler) {
    requests.getPopup = handler;
}

void CXdgSurface::setSetWindowGeometry(F<void(CXdgSurface*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.setWindowGeometry = handler;
}

void CXdgSurface::setAckConfigure(F<void(CXdgSurface*, uint32_t)> handler) {
    requests.ackConfigure = handler;
}

static void _CXdgToplevelDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgToplevelSetParent(wl_client* client, wl_resource* resource, wl_resource* parent) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setParent)
        PO->requests.setParent(PO, parent);
}

static void _CXdgToplevelSetTitle(wl_client* client, wl_resource* resource, const char* title) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setTitle)
        PO->requests.setTitle(PO, title);
}

static void _CXdgToplevelSetAppId(wl_client* client, wl_resource* resource, const char* app_id) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setAppId)
        PO->requests.setAppId(PO, app_id);
}

static void _CXdgToplevelShowWindowMenu(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial, int32_t x, int32_t y) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.showWindowMenu)
        PO->requests.showWindowMenu(PO, seat, serial, x, y);
}

static void _CXdgToplevelMove(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.move)
        PO->requests.move(PO, seat, serial);
}

static void _CXdgToplevelResize(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial, xdgToplevelResizeEdge edges) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.resize)
        PO->requests.resize(PO, seat, serial, edges);
}

static void _CXdgToplevelSetMaxSize(wl_client* client, wl_resource* resource, int32_t width, int32_t height) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMaxSize)
        PO->requests.setMaxSize(PO, width, height);
}

static void _CXdgToplevelSetMinSize(wl_client* client, wl_resource* resource, int32_t width, int32_t height) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMinSize)
        PO->requests.setMinSize(PO, width, height);
}

static void _CXdgToplevelSetMaximized(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMaximized)
        PO->requests.setMaximized(PO);
}

static void _CXdgToplevelUnsetMaximized(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unsetMaximized)
        PO->requests.unsetMaximized(PO);
}

static void _CXdgToplevelSetFullscreen(wl_client* client, wl_resource* resource, wl_resource* output) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setFullscreen)
        PO->requests.setFullscreen(PO, output);
}

static void _CXdgToplevelUnsetFullscreen(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unsetFullscreen)
        PO->requests.unsetFullscreen(PO);
}

static void _CXdgToplevelSetMinimized(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgToplevel*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMinimized)
        PO->requests.setMinimized(PO);
}

static void _CXdgToplevel__DestroyListener(wl_listener* l, void* d) {
    CXdgToplevelDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgToplevel* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgToplevelVTable[] = {
    (void*)_CXdgToplevelDestroy,
    (void*)_CXdgToplevelSetParent,
    (void*)_CXdgToplevelSetTitle,
    (void*)_CXdgToplevelSetAppId,
    (void*)_CXdgToplevelShowWindowMenu,
    (void*)_CXdgToplevelMove,
    (void*)_CXdgToplevelResize,
    (void*)_CXdgToplevelSetMaxSize,
    (void*)_CXdgToplevelSetMinSize,
    (void*)_CXdgToplevelSetMaximized,
    (void*)_CXdgToplevelUnsetMaximized,
    (void*)_CXdgToplevelSetFullscreen,
    (void*)_CXdgToplevelUnsetFullscreen,
    (void*)_CXdgToplevelSetMinimized,
};

void CXdgToplevel::sendConfigure(int32_t width, int32_t height, wl_array* states) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, width, height, states);
}

void CXdgToplevel::sendClose() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CXdgToplevel::sendConfigureBounds(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, width, height);
}

void CXdgToplevel::sendWmCapabilities(wl_array* capabilities) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, capabilities);
}

void CXdgToplevel::sendConfigureRaw(int32_t width, int32_t height, wl_array* states) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, width, height, states);
}

void CXdgToplevel::sendCloseRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CXdgToplevel::sendConfigureBoundsRaw(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, width, height);
}

void CXdgToplevel::sendWmCapabilitiesRaw(wl_array* capabilities) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, capabilities);
}
static const wl_interface* _CXdgToplevelSetParentTypes[] = {
    &xdg_toplevel_interface,
};
static const wl_interface* _CXdgToplevelSetTitleTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgToplevelSetAppIdTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgToplevelShowWindowMenuTypes[] = {
    &wl_seat_interface,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgToplevelMoveTypes[] = {
    &wl_seat_interface,
    nullptr,
};
static const wl_interface* _CXdgToplevelResizeTypes[] = {
    &wl_seat_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgToplevelSetMaxSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgToplevelSetMinSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgToplevelSetFullscreenTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CXdgToplevelConfigureTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgToplevelConfigureBoundsTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgToplevelWmCapabilitiesTypes[] = {
    nullptr,
};

static const wl_message _CXdgToplevelRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_parent", "?o", _CXdgToplevelSetParentTypes + 0},
    { "set_title", "s", _CXdgToplevelSetTitleTypes + 0},
    { "set_app_id", "s", _CXdgToplevelSetAppIdTypes + 0},
    { "show_window_menu", "ouii", _CXdgToplevelShowWindowMenuTypes + 0},
    { "move", "ou", _CXdgToplevelMoveTypes + 0},
    { "resize", "ouu", _CXdgToplevelResizeTypes + 0},
    { "set_max_size", "ii", _CXdgToplevelSetMaxSizeTypes + 0},
    { "set_min_size", "ii", _CXdgToplevelSetMinSizeTypes + 0},
    { "set_maximized", "", dummyTypes + 0},
    { "unset_maximized", "", dummyTypes + 0},
    { "set_fullscreen", "?o", _CXdgToplevelSetFullscreenTypes + 0},
    { "unset_fullscreen", "", dummyTypes + 0},
    { "set_minimized", "", dummyTypes + 0},
};

static const wl_message _CXdgToplevelEvents[] = {
    { "configure", "iia", _CXdgToplevelConfigureTypes + 0},
    { "close", "", dummyTypes + 0},
    { "configure_bounds", "4ii", _CXdgToplevelConfigureBoundsTypes + 0},
    { "wm_capabilities", "5a", _CXdgToplevelWmCapabilitiesTypes + 0},
};

const wl_interface xdg_toplevel_interface = {
    "xdg_toplevel", 6,
    14, _CXdgToplevelRequests,
    4, _CXdgToplevelEvents,
};

CXdgToplevel::CXdgToplevel(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_toplevel_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgToplevel__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgToplevelVTable, this, nullptr);
}

CXdgToplevel::~CXdgToplevel() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgToplevel::onDestroyCalled() {
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

void CXdgToplevel::setDestroy(F<void(CXdgToplevel*)> handler) {
    requests.destroy = handler;
}

void CXdgToplevel::setSetParent(F<void(CXdgToplevel*, wl_resource*)> handler) {
    requests.setParent = handler;
}

void CXdgToplevel::setSetTitle(F<void(CXdgToplevel*, const char*)> handler) {
    requests.setTitle = handler;
}

void CXdgToplevel::setSetAppId(F<void(CXdgToplevel*, const char*)> handler) {
    requests.setAppId = handler;
}

void CXdgToplevel::setShowWindowMenu(F<void(CXdgToplevel*, wl_resource*, uint32_t, int32_t, int32_t)> handler) {
    requests.showWindowMenu = handler;
}

void CXdgToplevel::setMove(F<void(CXdgToplevel*, wl_resource*, uint32_t)> handler) {
    requests.move = handler;
}

void CXdgToplevel::setResize(F<void(CXdgToplevel*, wl_resource*, uint32_t, xdgToplevelResizeEdge)> handler) {
    requests.resize = handler;
}

void CXdgToplevel::setSetMaxSize(F<void(CXdgToplevel*, int32_t, int32_t)> handler) {
    requests.setMaxSize = handler;
}

void CXdgToplevel::setSetMinSize(F<void(CXdgToplevel*, int32_t, int32_t)> handler) {
    requests.setMinSize = handler;
}

void CXdgToplevel::setSetMaximized(F<void(CXdgToplevel*)> handler) {
    requests.setMaximized = handler;
}

void CXdgToplevel::setUnsetMaximized(F<void(CXdgToplevel*)> handler) {
    requests.unsetMaximized = handler;
}

void CXdgToplevel::setSetFullscreen(F<void(CXdgToplevel*, wl_resource*)> handler) {
    requests.setFullscreen = handler;
}

void CXdgToplevel::setUnsetFullscreen(F<void(CXdgToplevel*)> handler) {
    requests.unsetFullscreen = handler;
}

void CXdgToplevel::setSetMinimized(F<void(CXdgToplevel*)> handler) {
    requests.setMinimized = handler;
}

static void _CXdgPopupDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgPopup*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgPopupGrab(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial) {
    const auto PO = (CXdgPopup*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.grab)
        PO->requests.grab(PO, seat, serial);
}

static void _CXdgPopupReposition(wl_client* client, wl_resource* resource, wl_resource* positioner, uint32_t token) {
    const auto PO = (CXdgPopup*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.reposition)
        PO->requests.reposition(PO, positioner, token);
}

static void _CXdgPopup__DestroyListener(wl_listener* l, void* d) {
    CXdgPopupDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgPopup* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgPopupVTable[] = {
    (void*)_CXdgPopupDestroy,
    (void*)_CXdgPopupGrab,
    (void*)_CXdgPopupReposition,
};

void CXdgPopup::sendConfigure(int32_t x, int32_t y, int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y, width, height);
}

void CXdgPopup::sendPopupDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CXdgPopup::sendRepositioned(uint32_t token) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, token);
}

void CXdgPopup::sendConfigureRaw(int32_t x, int32_t y, int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y, width, height);
}

void CXdgPopup::sendPopupDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CXdgPopup::sendRepositionedRaw(uint32_t token) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, token);
}
static const wl_interface* _CXdgPopupGrabTypes[] = {
    &wl_seat_interface,
    nullptr,
};
static const wl_interface* _CXdgPopupRepositionTypes[] = {
    &xdg_positioner_interface,
    nullptr,
};
static const wl_interface* _CXdgPopupConfigureTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CXdgPopupRepositionedTypes[] = {
    nullptr,
};

static const wl_message _CXdgPopupRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "grab", "ou", _CXdgPopupGrabTypes + 0},
    { "reposition", "3ou", _CXdgPopupRepositionTypes + 0},
};

static const wl_message _CXdgPopupEvents[] = {
    { "configure", "iiii", _CXdgPopupConfigureTypes + 0},
    { "popup_done", "", dummyTypes + 0},
    { "repositioned", "3u", _CXdgPopupRepositionedTypes + 0},
};

const wl_interface xdg_popup_interface = {
    "xdg_popup", 6,
    3, _CXdgPopupRequests,
    3, _CXdgPopupEvents,
};

CXdgPopup::CXdgPopup(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_popup_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgPopup__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgPopupVTable, this, nullptr);
}

CXdgPopup::~CXdgPopup() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgPopup::onDestroyCalled() {
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

void CXdgPopup::setDestroy(F<void(CXdgPopup*)> handler) {
    requests.destroy = handler;
}

void CXdgPopup::setGrab(F<void(CXdgPopup*, wl_resource*, uint32_t)> handler) {
    requests.grab = handler;
}

void CXdgPopup::setReposition(F<void(CXdgPopup*, wl_resource*, uint32_t)> handler) {
    requests.reposition = handler;
}

#undef F
