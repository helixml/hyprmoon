// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// keyboard_shortcuts_inhibit_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2017 Red Hat Inc.

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
#include "keyboard-shortcuts-inhibit-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_keyboard_shortcuts_inhibit_manager_v1_interface;
extern const wl_interface zwp_keyboard_shortcuts_inhibitor_v1_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_seat_interface;

static void _CZwpKeyboardShortcutsInhibitManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpKeyboardShortcutsInhibitManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpKeyboardShortcutsInhibitManagerV1InhibitShortcuts(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface, wl_resource* seat) {
    const auto PO = (CZwpKeyboardShortcutsInhibitManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.inhibitShortcuts)
        PO->requests.inhibitShortcuts(PO, id, surface, seat);
}

static void _CZwpKeyboardShortcutsInhibitManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwpKeyboardShortcutsInhibitManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpKeyboardShortcutsInhibitManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpKeyboardShortcutsInhibitManagerV1VTable[] = {
    (void*)_CZwpKeyboardShortcutsInhibitManagerV1Destroy,
    (void*)_CZwpKeyboardShortcutsInhibitManagerV1InhibitShortcuts,
};
static const wl_interface* _CZwpKeyboardShortcutsInhibitManagerV1InhibitShortcutsTypes[] = {
    &zwp_keyboard_shortcuts_inhibitor_v1_interface,
    &wl_surface_interface,
    &wl_seat_interface,
};

static const wl_message _CZwpKeyboardShortcutsInhibitManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "inhibit_shortcuts", "noo", _CZwpKeyboardShortcutsInhibitManagerV1InhibitShortcutsTypes + 0},
};

const wl_interface zwp_keyboard_shortcuts_inhibit_manager_v1_interface = {
    "zwp_keyboard_shortcuts_inhibit_manager_v1", 1,
    2, _CZwpKeyboardShortcutsInhibitManagerV1Requests,
    0, nullptr,
};

CZwpKeyboardShortcutsInhibitManagerV1::CZwpKeyboardShortcutsInhibitManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_keyboard_shortcuts_inhibit_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpKeyboardShortcutsInhibitManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpKeyboardShortcutsInhibitManagerV1VTable, this, nullptr);
}

CZwpKeyboardShortcutsInhibitManagerV1::~CZwpKeyboardShortcutsInhibitManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpKeyboardShortcutsInhibitManagerV1::onDestroyCalled() {
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

void CZwpKeyboardShortcutsInhibitManagerV1::setDestroy(F<void(CZwpKeyboardShortcutsInhibitManagerV1*)> handler) {
    requests.destroy = handler;
}

void CZwpKeyboardShortcutsInhibitManagerV1::setInhibitShortcuts(F<void(CZwpKeyboardShortcutsInhibitManagerV1*, uint32_t, wl_resource*, wl_resource*)> handler) {
    requests.inhibitShortcuts = handler;
}

static void _CZwpKeyboardShortcutsInhibitorV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpKeyboardShortcutsInhibitorV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpKeyboardShortcutsInhibitorV1__DestroyListener(wl_listener* l, void* d) {
    CZwpKeyboardShortcutsInhibitorV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpKeyboardShortcutsInhibitorV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpKeyboardShortcutsInhibitorV1VTable[] = {
    (void*)_CZwpKeyboardShortcutsInhibitorV1Destroy,
};

void CZwpKeyboardShortcutsInhibitorV1::sendActive() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CZwpKeyboardShortcutsInhibitorV1::sendInactive() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwpKeyboardShortcutsInhibitorV1::sendActiveRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CZwpKeyboardShortcutsInhibitorV1::sendInactiveRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

static const wl_message _CZwpKeyboardShortcutsInhibitorV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpKeyboardShortcutsInhibitorV1Events[] = {
    { "active", "", dummyTypes + 0},
    { "inactive", "", dummyTypes + 0},
};

const wl_interface zwp_keyboard_shortcuts_inhibitor_v1_interface = {
    "zwp_keyboard_shortcuts_inhibitor_v1", 1,
    1, _CZwpKeyboardShortcutsInhibitorV1Requests,
    2, _CZwpKeyboardShortcutsInhibitorV1Events,
};

CZwpKeyboardShortcutsInhibitorV1::CZwpKeyboardShortcutsInhibitorV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_keyboard_shortcuts_inhibitor_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpKeyboardShortcutsInhibitorV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpKeyboardShortcutsInhibitorV1VTable, this, nullptr);
}

CZwpKeyboardShortcutsInhibitorV1::~CZwpKeyboardShortcutsInhibitorV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpKeyboardShortcutsInhibitorV1::onDestroyCalled() {
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

void CZwpKeyboardShortcutsInhibitorV1::setDestroy(F<void(CZwpKeyboardShortcutsInhibitorV1*)> handler) {
    requests.destroy = handler;
}

#undef F
