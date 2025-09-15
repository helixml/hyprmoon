// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// input_method_unstable_v2

/*
 This protocol's authors' copyright notice is:


    Copyright © 2008-2011 Kristian Høgsberg
    Copyright © 2010-2011 Intel Corporation
    Copyright © 2012-2013 Collabora, Ltd.
    Copyright © 2012, 2013 Intel Corporation
    Copyright © 2015, 2016 Jan Arne Petersen
    Copyright © 2017, 2018 Red Hat, Inc.
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
#include "input-method-unstable-v2.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_input_method_v2_interface;
extern const wl_interface zwp_input_popup_surface_v2_interface;
extern const wl_interface zwp_input_method_keyboard_grab_v2_interface;
extern const wl_interface zwp_input_method_manager_v2_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_seat_interface;

static void _CZwpInputMethodV2CommitString(wl_client* client, wl_resource* resource, const char* text) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.commitString)
        PO->requests.commitString(PO, text);
}

static void _CZwpInputMethodV2SetPreeditString(wl_client* client, wl_resource* resource, const char* text, int32_t cursor_begin, int32_t cursor_end) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setPreeditString)
        PO->requests.setPreeditString(PO, text, cursor_begin, cursor_end);
}

static void _CZwpInputMethodV2DeleteSurroundingText(wl_client* client, wl_resource* resource, uint32_t before_length, uint32_t after_length) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.deleteSurroundingText)
        PO->requests.deleteSurroundingText(PO, before_length, after_length);
}

static void _CZwpInputMethodV2Commit(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.commit)
        PO->requests.commit(PO, serial);
}

static void _CZwpInputMethodV2GetInputPopupSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getInputPopupSurface)
        PO->requests.getInputPopupSurface(PO, id, surface);
}

static void _CZwpInputMethodV2GrabKeyboard(wl_client* client, wl_resource* resource, uint32_t keyboard) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.grabKeyboard)
        PO->requests.grabKeyboard(PO, keyboard);
}

static void _CZwpInputMethodV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpInputMethodV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpInputMethodV2__DestroyListener(wl_listener* l, void* d) {
    CZwpInputMethodV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpInputMethodV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpInputMethodV2VTable[] = {
    (void*)_CZwpInputMethodV2CommitString,
    (void*)_CZwpInputMethodV2SetPreeditString,
    (void*)_CZwpInputMethodV2DeleteSurroundingText,
    (void*)_CZwpInputMethodV2Commit,
    (void*)_CZwpInputMethodV2GetInputPopupSurface,
    (void*)_CZwpInputMethodV2GrabKeyboard,
    (void*)_CZwpInputMethodV2Destroy,
};

void CZwpInputMethodV2::sendActivate() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CZwpInputMethodV2::sendDeactivate() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwpInputMethodV2::sendSurroundingText(const char* text, uint32_t cursor, uint32_t anchor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, text, cursor, anchor);
}

void CZwpInputMethodV2::sendTextChangeCause(uint32_t cause) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, cause);
}

void CZwpInputMethodV2::sendContentType(uint32_t hint, uint32_t purpose) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, hint, purpose);
}

void CZwpInputMethodV2::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CZwpInputMethodV2::sendUnavailable() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6);
}

void CZwpInputMethodV2::sendActivateRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CZwpInputMethodV2::sendDeactivateRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwpInputMethodV2::sendSurroundingTextRaw(const char* text, uint32_t cursor, uint32_t anchor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, text, cursor, anchor);
}

void CZwpInputMethodV2::sendTextChangeCauseRaw(uint32_t cause) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, cause);
}

void CZwpInputMethodV2::sendContentTypeRaw(uint32_t hint, uint32_t purpose) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, hint, purpose);
}

void CZwpInputMethodV2::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CZwpInputMethodV2::sendUnavailableRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6);
}
static const wl_interface* _CZwpInputMethodV2CommitStringTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpInputMethodV2SetPreeditStringTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpInputMethodV2DeleteSurroundingTextTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpInputMethodV2CommitTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpInputMethodV2GetInputPopupSurfaceTypes[] = {
    &zwp_input_popup_surface_v2_interface,
    &wl_surface_interface,
};
static const wl_interface* _CZwpInputMethodV2GrabKeyboardTypes[] = {
    &zwp_input_method_keyboard_grab_v2_interface,
};
static const wl_interface* _CZwpInputMethodV2SurroundingTextTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpInputMethodV2TextChangeCauseTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpInputMethodV2ContentTypeTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CZwpInputMethodV2Requests[] = {
    { "commit_string", "s", _CZwpInputMethodV2CommitStringTypes + 0},
    { "set_preedit_string", "sii", _CZwpInputMethodV2SetPreeditStringTypes + 0},
    { "delete_surrounding_text", "uu", _CZwpInputMethodV2DeleteSurroundingTextTypes + 0},
    { "commit", "u", _CZwpInputMethodV2CommitTypes + 0},
    { "get_input_popup_surface", "no", _CZwpInputMethodV2GetInputPopupSurfaceTypes + 0},
    { "grab_keyboard", "n", _CZwpInputMethodV2GrabKeyboardTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpInputMethodV2Events[] = {
    { "activate", "", dummyTypes + 0},
    { "deactivate", "", dummyTypes + 0},
    { "surrounding_text", "suu", _CZwpInputMethodV2SurroundingTextTypes + 0},
    { "text_change_cause", "u", _CZwpInputMethodV2TextChangeCauseTypes + 0},
    { "content_type", "uu", _CZwpInputMethodV2ContentTypeTypes + 0},
    { "done", "", dummyTypes + 0},
    { "unavailable", "", dummyTypes + 0},
};

const wl_interface zwp_input_method_v2_interface = {
    "zwp_input_method_v2", 1,
    7, _CZwpInputMethodV2Requests,
    7, _CZwpInputMethodV2Events,
};

CZwpInputMethodV2::CZwpInputMethodV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_input_method_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpInputMethodV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpInputMethodV2VTable, this, nullptr);
}

CZwpInputMethodV2::~CZwpInputMethodV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpInputMethodV2::onDestroyCalled() {
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

void CZwpInputMethodV2::setCommitString(F<void(CZwpInputMethodV2*, const char*)> handler) {
    requests.commitString = handler;
}

void CZwpInputMethodV2::setSetPreeditString(F<void(CZwpInputMethodV2*, const char*, int32_t, int32_t)> handler) {
    requests.setPreeditString = handler;
}

void CZwpInputMethodV2::setDeleteSurroundingText(F<void(CZwpInputMethodV2*, uint32_t, uint32_t)> handler) {
    requests.deleteSurroundingText = handler;
}

void CZwpInputMethodV2::setCommit(F<void(CZwpInputMethodV2*, uint32_t)> handler) {
    requests.commit = handler;
}

void CZwpInputMethodV2::setGetInputPopupSurface(F<void(CZwpInputMethodV2*, uint32_t, wl_resource*)> handler) {
    requests.getInputPopupSurface = handler;
}

void CZwpInputMethodV2::setGrabKeyboard(F<void(CZwpInputMethodV2*, uint32_t)> handler) {
    requests.grabKeyboard = handler;
}

void CZwpInputMethodV2::setDestroy(F<void(CZwpInputMethodV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpInputPopupSurfaceV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpInputPopupSurfaceV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpInputPopupSurfaceV2__DestroyListener(wl_listener* l, void* d) {
    CZwpInputPopupSurfaceV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpInputPopupSurfaceV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpInputPopupSurfaceV2VTable[] = {
    (void*)_CZwpInputPopupSurfaceV2Destroy,
};

void CZwpInputPopupSurfaceV2::sendTextInputRectangle(int32_t x, int32_t y, int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y, width, height);
}

void CZwpInputPopupSurfaceV2::sendTextInputRectangleRaw(int32_t x, int32_t y, int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y, width, height);
}
static const wl_interface* _CZwpInputPopupSurfaceV2TextInputRectangleTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwpInputPopupSurfaceV2Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpInputPopupSurfaceV2Events[] = {
    { "text_input_rectangle", "iiii", _CZwpInputPopupSurfaceV2TextInputRectangleTypes + 0},
};

const wl_interface zwp_input_popup_surface_v2_interface = {
    "zwp_input_popup_surface_v2", 1,
    1, _CZwpInputPopupSurfaceV2Requests,
    1, _CZwpInputPopupSurfaceV2Events,
};

CZwpInputPopupSurfaceV2::CZwpInputPopupSurfaceV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_input_popup_surface_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpInputPopupSurfaceV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpInputPopupSurfaceV2VTable, this, nullptr);
}

CZwpInputPopupSurfaceV2::~CZwpInputPopupSurfaceV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpInputPopupSurfaceV2::onDestroyCalled() {
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

void CZwpInputPopupSurfaceV2::setDestroy(F<void(CZwpInputPopupSurfaceV2*)> handler) {
    requests.destroy = handler;
}

static void _CZwpInputMethodKeyboardGrabV2Release(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpInputMethodKeyboardGrabV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CZwpInputMethodKeyboardGrabV2__DestroyListener(wl_listener* l, void* d) {
    CZwpInputMethodKeyboardGrabV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpInputMethodKeyboardGrabV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpInputMethodKeyboardGrabV2VTable[] = {
    (void*)_CZwpInputMethodKeyboardGrabV2Release,
};

void CZwpInputMethodKeyboardGrabV2::sendKeymap(uint32_t format, int32_t fd, uint32_t size) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, format, fd, size);
}

void CZwpInputMethodKeyboardGrabV2::sendKey(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, time, key, state);
}

void CZwpInputMethodKeyboardGrabV2::sendModifiers(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, mods_depressed, mods_latched, mods_locked, group);
}

void CZwpInputMethodKeyboardGrabV2::sendRepeatInfo(int32_t rate, int32_t delay) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, rate, delay);
}

void CZwpInputMethodKeyboardGrabV2::sendKeymapRaw(uint32_t format, int32_t fd, uint32_t size) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, format, fd, size);
}

void CZwpInputMethodKeyboardGrabV2::sendKeyRaw(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, time, key, state);
}

void CZwpInputMethodKeyboardGrabV2::sendModifiersRaw(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, mods_depressed, mods_latched, mods_locked, group);
}

void CZwpInputMethodKeyboardGrabV2::sendRepeatInfoRaw(int32_t rate, int32_t delay) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, rate, delay);
}
static const wl_interface* _CZwpInputMethodKeyboardGrabV2KeymapTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpInputMethodKeyboardGrabV2KeyTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpInputMethodKeyboardGrabV2ModifiersTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpInputMethodKeyboardGrabV2RepeatInfoTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CZwpInputMethodKeyboardGrabV2Requests[] = {
    { "release", "", dummyTypes + 0},
};

static const wl_message _CZwpInputMethodKeyboardGrabV2Events[] = {
    { "keymap", "uhu", _CZwpInputMethodKeyboardGrabV2KeymapTypes + 0},
    { "key", "uuuu", _CZwpInputMethodKeyboardGrabV2KeyTypes + 0},
    { "modifiers", "uuuuu", _CZwpInputMethodKeyboardGrabV2ModifiersTypes + 0},
    { "repeat_info", "ii", _CZwpInputMethodKeyboardGrabV2RepeatInfoTypes + 0},
};

const wl_interface zwp_input_method_keyboard_grab_v2_interface = {
    "zwp_input_method_keyboard_grab_v2", 1,
    1, _CZwpInputMethodKeyboardGrabV2Requests,
    4, _CZwpInputMethodKeyboardGrabV2Events,
};

CZwpInputMethodKeyboardGrabV2::CZwpInputMethodKeyboardGrabV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_input_method_keyboard_grab_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpInputMethodKeyboardGrabV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpInputMethodKeyboardGrabV2VTable, this, nullptr);
}

CZwpInputMethodKeyboardGrabV2::~CZwpInputMethodKeyboardGrabV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpInputMethodKeyboardGrabV2::onDestroyCalled() {
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

void CZwpInputMethodKeyboardGrabV2::setRelease(F<void(CZwpInputMethodKeyboardGrabV2*)> handler) {
    requests.release = handler;
}

static void _CZwpInputMethodManagerV2GetInputMethod(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t input_method) {
    const auto PO = (CZwpInputMethodManagerV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getInputMethod)
        PO->requests.getInputMethod(PO, seat, input_method);
}

static void _CZwpInputMethodManagerV2Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpInputMethodManagerV2*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpInputMethodManagerV2__DestroyListener(wl_listener* l, void* d) {
    CZwpInputMethodManagerV2DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpInputMethodManagerV2* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpInputMethodManagerV2VTable[] = {
    (void*)_CZwpInputMethodManagerV2GetInputMethod,
    (void*)_CZwpInputMethodManagerV2Destroy,
};
static const wl_interface* _CZwpInputMethodManagerV2GetInputMethodTypes[] = {
    &wl_seat_interface,
    &zwp_input_method_v2_interface,
};

static const wl_message _CZwpInputMethodManagerV2Requests[] = {
    { "get_input_method", "on", _CZwpInputMethodManagerV2GetInputMethodTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface zwp_input_method_manager_v2_interface = {
    "zwp_input_method_manager_v2", 1,
    2, _CZwpInputMethodManagerV2Requests,
    0, nullptr,
};

CZwpInputMethodManagerV2::CZwpInputMethodManagerV2(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_input_method_manager_v2_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpInputMethodManagerV2__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpInputMethodManagerV2VTable, this, nullptr);
}

CZwpInputMethodManagerV2::~CZwpInputMethodManagerV2() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpInputMethodManagerV2::onDestroyCalled() {
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

void CZwpInputMethodManagerV2::setGetInputMethod(F<void(CZwpInputMethodManagerV2*, wl_resource*, uint32_t)> handler) {
    requests.getInputMethod = handler;
}

void CZwpInputMethodManagerV2::setDestroy(F<void(CZwpInputMethodManagerV2*)> handler) {
    requests.destroy = handler;
}

#undef F
