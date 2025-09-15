// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// virtual_keyboard_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2008-2011  Kristian Høgsberg
    Copyright © 2010-2013  Intel Corporation
    Copyright © 2012-2013  Collabora, Ltd.
    Copyright © 2018       Purism SPC

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
#include "virtual-keyboard-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_virtual_keyboard_v1_interface;
extern const wl_interface zwp_virtual_keyboard_manager_v1_interface;
extern const wl_interface wl_seat_interface;

static void _CZwpVirtualKeyboardV1Keymap(wl_client* client, wl_resource* resource, uint32_t format, int32_t fd, uint32_t size) {
    const auto PO = (CZwpVirtualKeyboardV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.keymap)
        PO->requests.keymap(PO, format, fd, size);
}

static void _CZwpVirtualKeyboardV1Key(wl_client* client, wl_resource* resource, uint32_t time, uint32_t key, uint32_t state) {
    const auto PO = (CZwpVirtualKeyboardV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.key)
        PO->requests.key(PO, time, key, state);
}

static void _CZwpVirtualKeyboardV1Modifiers(wl_client* client, wl_resource* resource, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    const auto PO = (CZwpVirtualKeyboardV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.modifiers)
        PO->requests.modifiers(PO, mods_depressed, mods_latched, mods_locked, group);
}

static void _CZwpVirtualKeyboardV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpVirtualKeyboardV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpVirtualKeyboardV1__DestroyListener(wl_listener* l, void* d) {
    CZwpVirtualKeyboardV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpVirtualKeyboardV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpVirtualKeyboardV1VTable[] = {
    (void*)_CZwpVirtualKeyboardV1Keymap,
    (void*)_CZwpVirtualKeyboardV1Key,
    (void*)_CZwpVirtualKeyboardV1Modifiers,
    (void*)_CZwpVirtualKeyboardV1Destroy,
};
static const wl_interface* _CZwpVirtualKeyboardV1KeymapTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpVirtualKeyboardV1KeyTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpVirtualKeyboardV1ModifiersTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwpVirtualKeyboardV1Requests[] = {
    { "keymap", "uhu", _CZwpVirtualKeyboardV1KeymapTypes + 0},
    { "key", "uuu", _CZwpVirtualKeyboardV1KeyTypes + 0},
    { "modifiers", "uuuu", _CZwpVirtualKeyboardV1ModifiersTypes + 0},
    { "destroy", "1", dummyTypes + 0},
};

const wl_interface zwp_virtual_keyboard_v1_interface = {
    "zwp_virtual_keyboard_v1", 1,
    4, _CZwpVirtualKeyboardV1Requests,
    0, nullptr,
};

CZwpVirtualKeyboardV1::CZwpVirtualKeyboardV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_virtual_keyboard_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpVirtualKeyboardV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpVirtualKeyboardV1VTable, this, nullptr);
}

CZwpVirtualKeyboardV1::~CZwpVirtualKeyboardV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpVirtualKeyboardV1::onDestroyCalled() {
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

void CZwpVirtualKeyboardV1::setKeymap(F<void(CZwpVirtualKeyboardV1*, uint32_t, int32_t, uint32_t)> handler) {
    requests.keymap = handler;
}

void CZwpVirtualKeyboardV1::setKey(F<void(CZwpVirtualKeyboardV1*, uint32_t, uint32_t, uint32_t)> handler) {
    requests.key = handler;
}

void CZwpVirtualKeyboardV1::setModifiers(F<void(CZwpVirtualKeyboardV1*, uint32_t, uint32_t, uint32_t, uint32_t)> handler) {
    requests.modifiers = handler;
}

void CZwpVirtualKeyboardV1::setDestroy(F<void(CZwpVirtualKeyboardV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwpVirtualKeyboardManagerV1CreateVirtualKeyboard(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t id) {
    const auto PO = (CZwpVirtualKeyboardManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createVirtualKeyboard)
        PO->requests.createVirtualKeyboard(PO, seat, id);
}

static void _CZwpVirtualKeyboardManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwpVirtualKeyboardManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpVirtualKeyboardManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpVirtualKeyboardManagerV1VTable[] = {
    (void*)_CZwpVirtualKeyboardManagerV1CreateVirtualKeyboard,
};
static const wl_interface* _CZwpVirtualKeyboardManagerV1CreateVirtualKeyboardTypes[] = {
    &wl_seat_interface,
    &zwp_virtual_keyboard_v1_interface,
};

static const wl_message _CZwpVirtualKeyboardManagerV1Requests[] = {
    { "create_virtual_keyboard", "on", _CZwpVirtualKeyboardManagerV1CreateVirtualKeyboardTypes + 0},
};

const wl_interface zwp_virtual_keyboard_manager_v1_interface = {
    "zwp_virtual_keyboard_manager_v1", 1,
    1, _CZwpVirtualKeyboardManagerV1Requests,
    0, nullptr,
};

CZwpVirtualKeyboardManagerV1::CZwpVirtualKeyboardManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_virtual_keyboard_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpVirtualKeyboardManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpVirtualKeyboardManagerV1VTable, this, nullptr);
}

CZwpVirtualKeyboardManagerV1::~CZwpVirtualKeyboardManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpVirtualKeyboardManagerV1::onDestroyCalled() {
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

void CZwpVirtualKeyboardManagerV1::setCreateVirtualKeyboard(F<void(CZwpVirtualKeyboardManagerV1*, wl_resource*, uint32_t)> handler) {
    requests.createVirtualKeyboard = handler;
}

#undef F
