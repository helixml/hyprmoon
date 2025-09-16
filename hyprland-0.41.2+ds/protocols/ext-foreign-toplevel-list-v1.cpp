// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// ext_foreign_toplevel_list_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2018 Ilia Bozhinov
    Copyright © 2020 Isaac Freund
    Copyright © 2022 wb9688
    Copyright © 2023 i509VCB

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
#include "ext-foreign-toplevel-list-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface ext_foreign_toplevel_list_v1_interface;
extern const wl_interface ext_foreign_toplevel_handle_v1_interface;

static void _CExtForeignToplevelListV1Stop(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtForeignToplevelListV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.stop)
        PO->requests.stop(PO);
}

static void _CExtForeignToplevelListV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtForeignToplevelListV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtForeignToplevelListV1__DestroyListener(wl_listener* l, void* d) {
    CExtForeignToplevelListV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtForeignToplevelListV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtForeignToplevelListV1VTable[] = {
    (void*)_CExtForeignToplevelListV1Stop,
    (void*)_CExtForeignToplevelListV1Destroy,
};

void CExtForeignToplevelListV1::sendToplevel(CExtForeignToplevelHandleV1* toplevel) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, toplevel ? toplevel->pResource : nullptr);
}

void CExtForeignToplevelListV1::sendFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CExtForeignToplevelListV1::sendToplevelRaw(CExtForeignToplevelHandleV1* toplevel) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, toplevel);
}

void CExtForeignToplevelListV1::sendFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}
static const wl_interface* _CExtForeignToplevelListV1ToplevelTypes[] = {
    &ext_foreign_toplevel_handle_v1_interface,
};

static const wl_message _CExtForeignToplevelListV1Requests[] = {
    { "stop", "", dummyTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CExtForeignToplevelListV1Events[] = {
    { "toplevel", "n", _CExtForeignToplevelListV1ToplevelTypes + 0},
    { "finished", "", dummyTypes + 0},
};

const wl_interface ext_foreign_toplevel_list_v1_interface = {
    "ext_foreign_toplevel_list_v1", 1,
    2, _CExtForeignToplevelListV1Requests,
    2, _CExtForeignToplevelListV1Events,
};

CExtForeignToplevelListV1::CExtForeignToplevelListV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_foreign_toplevel_list_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtForeignToplevelListV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtForeignToplevelListV1VTable, this, nullptr);
}

CExtForeignToplevelListV1::~CExtForeignToplevelListV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtForeignToplevelListV1::onDestroyCalled() {
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

void CExtForeignToplevelListV1::setStop(F<void(CExtForeignToplevelListV1*)> handler) {
    requests.stop = handler;
}

void CExtForeignToplevelListV1::setDestroy(F<void(CExtForeignToplevelListV1*)> handler) {
    requests.destroy = handler;
}

static void _CExtForeignToplevelHandleV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtForeignToplevelHandleV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtForeignToplevelHandleV1__DestroyListener(wl_listener* l, void* d) {
    CExtForeignToplevelHandleV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtForeignToplevelHandleV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtForeignToplevelHandleV1VTable[] = {
    (void*)_CExtForeignToplevelHandleV1Destroy,
};

void CExtForeignToplevelHandleV1::sendClosed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CExtForeignToplevelHandleV1::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CExtForeignToplevelHandleV1::sendTitle(const char* title) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, title);
}

void CExtForeignToplevelHandleV1::sendAppId(const char* app_id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, app_id);
}

void CExtForeignToplevelHandleV1::sendIdentifier(const char* identifier) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, identifier);
}

void CExtForeignToplevelHandleV1::sendClosedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CExtForeignToplevelHandleV1::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CExtForeignToplevelHandleV1::sendTitleRaw(const char* title) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, title);
}

void CExtForeignToplevelHandleV1::sendAppIdRaw(const char* app_id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, app_id);
}

void CExtForeignToplevelHandleV1::sendIdentifierRaw(const char* identifier) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, identifier);
}
static const wl_interface* _CExtForeignToplevelHandleV1TitleTypes[] = {
    nullptr,
};
static const wl_interface* _CExtForeignToplevelHandleV1AppIdTypes[] = {
    nullptr,
};
static const wl_interface* _CExtForeignToplevelHandleV1IdentifierTypes[] = {
    nullptr,
};

static const wl_message _CExtForeignToplevelHandleV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CExtForeignToplevelHandleV1Events[] = {
    { "closed", "", dummyTypes + 0},
    { "done", "", dummyTypes + 0},
    { "title", "s", _CExtForeignToplevelHandleV1TitleTypes + 0},
    { "app_id", "s", _CExtForeignToplevelHandleV1AppIdTypes + 0},
    { "identifier", "s", _CExtForeignToplevelHandleV1IdentifierTypes + 0},
};

const wl_interface ext_foreign_toplevel_handle_v1_interface = {
    "ext_foreign_toplevel_handle_v1", 1,
    1, _CExtForeignToplevelHandleV1Requests,
    5, _CExtForeignToplevelHandleV1Events,
};

CExtForeignToplevelHandleV1::CExtForeignToplevelHandleV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_foreign_toplevel_handle_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtForeignToplevelHandleV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtForeignToplevelHandleV1VTable, this, nullptr);
}

CExtForeignToplevelHandleV1::~CExtForeignToplevelHandleV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtForeignToplevelHandleV1::onDestroyCalled() {
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

void CExtForeignToplevelHandleV1::setDestroy(F<void(CExtForeignToplevelHandleV1*)> handler) {
    requests.destroy = handler;
}

#undef F
