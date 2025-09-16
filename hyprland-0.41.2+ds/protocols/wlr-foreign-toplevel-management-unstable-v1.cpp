// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wlr_foreign_toplevel_management_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2018 Ilia Bozhinov

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
#include "wlr-foreign-toplevel-management-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwlr_foreign_toplevel_manager_v1_interface;
extern const wl_interface zwlr_foreign_toplevel_handle_v1_interface;
extern const wl_interface wl_seat_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_output_interface;

static void _CZwlrForeignToplevelManagerV1Stop(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.stop)
        PO->requests.stop(PO);
}

static void _CZwlrForeignToplevelManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrForeignToplevelManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrForeignToplevelManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrForeignToplevelManagerV1VTable[] = {
    (void*)_CZwlrForeignToplevelManagerV1Stop,
};

void CZwlrForeignToplevelManagerV1::sendToplevel(CZwlrForeignToplevelHandleV1* toplevel) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, toplevel ? toplevel->pResource : nullptr);
}

void CZwlrForeignToplevelManagerV1::sendFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwlrForeignToplevelManagerV1::sendToplevelRaw(CZwlrForeignToplevelHandleV1* toplevel) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, toplevel);
}

void CZwlrForeignToplevelManagerV1::sendFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}
static const wl_interface* _CZwlrForeignToplevelManagerV1ToplevelTypes[] = {
    &zwlr_foreign_toplevel_handle_v1_interface,
};

static const wl_message _CZwlrForeignToplevelManagerV1Requests[] = {
    { "stop", "", dummyTypes + 0},
};

static const wl_message _CZwlrForeignToplevelManagerV1Events[] = {
    { "toplevel", "n", _CZwlrForeignToplevelManagerV1ToplevelTypes + 0},
    { "finished", "", dummyTypes + 0},
};

const wl_interface zwlr_foreign_toplevel_manager_v1_interface = {
    "zwlr_foreign_toplevel_manager_v1", 3,
    1, _CZwlrForeignToplevelManagerV1Requests,
    2, _CZwlrForeignToplevelManagerV1Events,
};

CZwlrForeignToplevelManagerV1::CZwlrForeignToplevelManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_foreign_toplevel_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrForeignToplevelManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrForeignToplevelManagerV1VTable, this, nullptr);
}

CZwlrForeignToplevelManagerV1::~CZwlrForeignToplevelManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrForeignToplevelManagerV1::onDestroyCalled() {
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

void CZwlrForeignToplevelManagerV1::setStop(F<void(CZwlrForeignToplevelManagerV1*)> handler) {
    requests.stop = handler;
}

static void _CZwlrForeignToplevelHandleV1SetMaximized(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMaximized)
        PO->requests.setMaximized(PO);
}

static void _CZwlrForeignToplevelHandleV1UnsetMaximized(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unsetMaximized)
        PO->requests.unsetMaximized(PO);
}

static void _CZwlrForeignToplevelHandleV1SetMinimized(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMinimized)
        PO->requests.setMinimized(PO);
}

static void _CZwlrForeignToplevelHandleV1UnsetMinimized(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unsetMinimized)
        PO->requests.unsetMinimized(PO);
}

static void _CZwlrForeignToplevelHandleV1Activate(wl_client* client, wl_resource* resource, wl_resource* seat) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.activate)
        PO->requests.activate(PO, seat);
}

static void _CZwlrForeignToplevelHandleV1Close(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.close)
        PO->requests.close(PO);
}

static void _CZwlrForeignToplevelHandleV1SetRectangle(wl_client* client, wl_resource* resource, wl_resource* surface, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setRectangle)
        PO->requests.setRectangle(PO, surface, x, y, width, height);
}

static void _CZwlrForeignToplevelHandleV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrForeignToplevelHandleV1SetFullscreen(wl_client* client, wl_resource* resource, wl_resource* output) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setFullscreen)
        PO->requests.setFullscreen(PO, output);
}

static void _CZwlrForeignToplevelHandleV1UnsetFullscreen(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unsetFullscreen)
        PO->requests.unsetFullscreen(PO);
}

static void _CZwlrForeignToplevelHandleV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrForeignToplevelHandleV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrForeignToplevelHandleV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrForeignToplevelHandleV1VTable[] = {
    (void*)_CZwlrForeignToplevelHandleV1SetMaximized,
    (void*)_CZwlrForeignToplevelHandleV1UnsetMaximized,
    (void*)_CZwlrForeignToplevelHandleV1SetMinimized,
    (void*)_CZwlrForeignToplevelHandleV1UnsetMinimized,
    (void*)_CZwlrForeignToplevelHandleV1Activate,
    (void*)_CZwlrForeignToplevelHandleV1Close,
    (void*)_CZwlrForeignToplevelHandleV1SetRectangle,
    (void*)_CZwlrForeignToplevelHandleV1Destroy,
    (void*)_CZwlrForeignToplevelHandleV1SetFullscreen,
    (void*)_CZwlrForeignToplevelHandleV1UnsetFullscreen,
};

void CZwlrForeignToplevelHandleV1::sendTitle(const char* title) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, title);
}

void CZwlrForeignToplevelHandleV1::sendAppId(const char* app_id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, app_id);
}

void CZwlrForeignToplevelHandleV1::sendOutputEnter(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, output);
}

void CZwlrForeignToplevelHandleV1::sendOutputLeave(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, output);
}

void CZwlrForeignToplevelHandleV1::sendState(wl_array* state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, state);
}

void CZwlrForeignToplevelHandleV1::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CZwlrForeignToplevelHandleV1::sendClosed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6);
}

void CZwlrForeignToplevelHandleV1::sendParent(CZwlrForeignToplevelHandleV1* parent) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7, parent ? parent->pResource : nullptr);
}

void CZwlrForeignToplevelHandleV1::sendTitleRaw(const char* title) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, title);
}

void CZwlrForeignToplevelHandleV1::sendAppIdRaw(const char* app_id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, app_id);
}

void CZwlrForeignToplevelHandleV1::sendOutputEnterRaw(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, output);
}

void CZwlrForeignToplevelHandleV1::sendOutputLeaveRaw(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, output);
}

void CZwlrForeignToplevelHandleV1::sendStateRaw(wl_array* state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, state);
}

void CZwlrForeignToplevelHandleV1::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CZwlrForeignToplevelHandleV1::sendClosedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6);
}

void CZwlrForeignToplevelHandleV1::sendParentRaw(wl_resource* parent) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7, parent);
}
static const wl_interface* _CZwlrForeignToplevelHandleV1ActivateTypes[] = {
    &wl_seat_interface,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1SetRectangleTypes[] = {
    &wl_surface_interface,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1SetFullscreenTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1TitleTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1AppIdTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1OutputEnterTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1OutputLeaveTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1StateTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrForeignToplevelHandleV1ParentTypes[] = {
    &zwlr_foreign_toplevel_handle_v1_interface,
};

static const wl_message _CZwlrForeignToplevelHandleV1Requests[] = {
    { "set_maximized", "", dummyTypes + 0},
    { "unset_maximized", "", dummyTypes + 0},
    { "set_minimized", "", dummyTypes + 0},
    { "unset_minimized", "", dummyTypes + 0},
    { "activate", "o", _CZwlrForeignToplevelHandleV1ActivateTypes + 0},
    { "close", "", dummyTypes + 0},
    { "set_rectangle", "oiiii", _CZwlrForeignToplevelHandleV1SetRectangleTypes + 0},
    { "destroy", "", dummyTypes + 0},
    { "set_fullscreen", "2?o", _CZwlrForeignToplevelHandleV1SetFullscreenTypes + 0},
    { "unset_fullscreen", "2", dummyTypes + 0},
};

static const wl_message _CZwlrForeignToplevelHandleV1Events[] = {
    { "title", "s", _CZwlrForeignToplevelHandleV1TitleTypes + 0},
    { "app_id", "s", _CZwlrForeignToplevelHandleV1AppIdTypes + 0},
    { "output_enter", "o", _CZwlrForeignToplevelHandleV1OutputEnterTypes + 0},
    { "output_leave", "o", _CZwlrForeignToplevelHandleV1OutputLeaveTypes + 0},
    { "state", "a", _CZwlrForeignToplevelHandleV1StateTypes + 0},
    { "done", "", dummyTypes + 0},
    { "closed", "", dummyTypes + 0},
    { "parent", "3?o", _CZwlrForeignToplevelHandleV1ParentTypes + 0},
};

const wl_interface zwlr_foreign_toplevel_handle_v1_interface = {
    "zwlr_foreign_toplevel_handle_v1", 3,
    10, _CZwlrForeignToplevelHandleV1Requests,
    8, _CZwlrForeignToplevelHandleV1Events,
};

CZwlrForeignToplevelHandleV1::CZwlrForeignToplevelHandleV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_foreign_toplevel_handle_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrForeignToplevelHandleV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrForeignToplevelHandleV1VTable, this, nullptr);
}

CZwlrForeignToplevelHandleV1::~CZwlrForeignToplevelHandleV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrForeignToplevelHandleV1::onDestroyCalled() {
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

void CZwlrForeignToplevelHandleV1::setSetMaximized(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.setMaximized = handler;
}

void CZwlrForeignToplevelHandleV1::setUnsetMaximized(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.unsetMaximized = handler;
}

void CZwlrForeignToplevelHandleV1::setSetMinimized(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.setMinimized = handler;
}

void CZwlrForeignToplevelHandleV1::setUnsetMinimized(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.unsetMinimized = handler;
}

void CZwlrForeignToplevelHandleV1::setActivate(F<void(CZwlrForeignToplevelHandleV1*, wl_resource*)> handler) {
    requests.activate = handler;
}

void CZwlrForeignToplevelHandleV1::setClose(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.close = handler;
}

void CZwlrForeignToplevelHandleV1::setSetRectangle(F<void(CZwlrForeignToplevelHandleV1*, wl_resource*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.setRectangle = handler;
}

void CZwlrForeignToplevelHandleV1::setDestroy(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.destroy = handler;
}

void CZwlrForeignToplevelHandleV1::setSetFullscreen(F<void(CZwlrForeignToplevelHandleV1*, wl_resource*)> handler) {
    requests.setFullscreen = handler;
}

void CZwlrForeignToplevelHandleV1::setUnsetFullscreen(F<void(CZwlrForeignToplevelHandleV1*)> handler) {
    requests.unsetFullscreen = handler;
}

#undef F
