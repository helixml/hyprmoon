// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// text_input_unstable_v3

/*
 This protocol's authors' copyright notice is:


    Copyright © 2012, 2013 Intel Corporation
    Copyright © 2015, 2016 Jan Arne Petersen
    Copyright © 2017, 2018 Red Hat, Inc.
    Copyright © 2018       Purism SPC

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
#include "text-input-unstable-v3.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_text_input_v3_interface;
extern const wl_interface zwp_text_input_manager_v3_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_seat_interface;

static void _CZwpTextInputV3Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTextInputV3Enable(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.enable)
        PO->requests.enable(PO);
}

static void _CZwpTextInputV3Disable(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.disable)
        PO->requests.disable(PO);
}

static void _CZwpTextInputV3SetSurroundingText(wl_client* client, wl_resource* resource, const char* text, int32_t cursor, int32_t anchor) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSurroundingText)
        PO->requests.setSurroundingText(PO, text, cursor, anchor);
}

static void _CZwpTextInputV3SetTextChangeCause(wl_client* client, wl_resource* resource, zwpTextInputV3ChangeCause cause) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setTextChangeCause)
        PO->requests.setTextChangeCause(PO, cause);
}

static void _CZwpTextInputV3SetContentType(wl_client* client, wl_resource* resource, zwpTextInputV3ContentHint hint, zwpTextInputV3ContentPurpose purpose) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setContentType)
        PO->requests.setContentType(PO, hint, purpose);
}

static void _CZwpTextInputV3SetCursorRectangle(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setCursorRectangle)
        PO->requests.setCursorRectangle(PO, x, y, width, height);
}

static void _CZwpTextInputV3Commit(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTextInputV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.commit)
        PO->requests.commit(PO);
}

static void _CZwpTextInputV3__DestroyListener(wl_listener* l, void* d) {
    CZwpTextInputV3DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTextInputV3* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTextInputV3VTable[] = {
    (void*)_CZwpTextInputV3Destroy,
    (void*)_CZwpTextInputV3Enable,
    (void*)_CZwpTextInputV3Disable,
    (void*)_CZwpTextInputV3SetSurroundingText,
    (void*)_CZwpTextInputV3SetTextChangeCause,
    (void*)_CZwpTextInputV3SetContentType,
    (void*)_CZwpTextInputV3SetCursorRectangle,
    (void*)_CZwpTextInputV3Commit,
};

void CZwpTextInputV3::sendEnter(wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, surface);
}

void CZwpTextInputV3::sendLeave(wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, surface);
}

void CZwpTextInputV3::sendPreeditString(const char* text, int32_t cursor_begin, int32_t cursor_end) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, text, cursor_begin, cursor_end);
}

void CZwpTextInputV3::sendCommitString(const char* text) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, text);
}

void CZwpTextInputV3::sendDeleteSurroundingText(uint32_t before_length, uint32_t after_length) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, before_length, after_length);
}

void CZwpTextInputV3::sendDone(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, serial);
}

void CZwpTextInputV3::sendEnterRaw(wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, surface);
}

void CZwpTextInputV3::sendLeaveRaw(wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, surface);
}

void CZwpTextInputV3::sendPreeditStringRaw(const char* text, int32_t cursor_begin, int32_t cursor_end) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, text, cursor_begin, cursor_end);
}

void CZwpTextInputV3::sendCommitStringRaw(const char* text) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, text);
}

void CZwpTextInputV3::sendDeleteSurroundingTextRaw(uint32_t before_length, uint32_t after_length) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, before_length, after_length);
}

void CZwpTextInputV3::sendDoneRaw(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, serial);
}
static const wl_interface* _CZwpTextInputV3SetSurroundingTextTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTextInputV3SetTextChangeCauseTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTextInputV3SetContentTypeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTextInputV3SetCursorRectangleTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTextInputV3EnterTypes[] = {
    &wl_surface_interface,
};
static const wl_interface* _CZwpTextInputV3LeaveTypes[] = {
    &wl_surface_interface,
};
static const wl_interface* _CZwpTextInputV3PreeditStringTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTextInputV3CommitStringTypes[] = {
    nullptr,
};
static const wl_interface* _CZwpTextInputV3DeleteSurroundingTextTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpTextInputV3DoneTypes[] = {
    nullptr,
};

static const wl_message _CZwpTextInputV3Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "enable", "", dummyTypes + 0},
    { "disable", "", dummyTypes + 0},
    { "set_surrounding_text", "sii", _CZwpTextInputV3SetSurroundingTextTypes + 0},
    { "set_text_change_cause", "u", _CZwpTextInputV3SetTextChangeCauseTypes + 0},
    { "set_content_type", "uu", _CZwpTextInputV3SetContentTypeTypes + 0},
    { "set_cursor_rectangle", "iiii", _CZwpTextInputV3SetCursorRectangleTypes + 0},
    { "commit", "", dummyTypes + 0},
};

static const wl_message _CZwpTextInputV3Events[] = {
    { "enter", "o", _CZwpTextInputV3EnterTypes + 0},
    { "leave", "o", _CZwpTextInputV3LeaveTypes + 0},
    { "preedit_string", "?sii", _CZwpTextInputV3PreeditStringTypes + 0},
    { "commit_string", "?s", _CZwpTextInputV3CommitStringTypes + 0},
    { "delete_surrounding_text", "uu", _CZwpTextInputV3DeleteSurroundingTextTypes + 0},
    { "done", "u", _CZwpTextInputV3DoneTypes + 0},
};

const wl_interface zwp_text_input_v3_interface = {
    "zwp_text_input_v3", 1,
    8, _CZwpTextInputV3Requests,
    6, _CZwpTextInputV3Events,
};

CZwpTextInputV3::CZwpTextInputV3(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_text_input_v3_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTextInputV3__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTextInputV3VTable, this, nullptr);
}

CZwpTextInputV3::~CZwpTextInputV3() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTextInputV3::onDestroyCalled() {
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

void CZwpTextInputV3::setDestroy(F<void(CZwpTextInputV3*)> handler) {
    requests.destroy = handler;
}

void CZwpTextInputV3::setEnable(F<void(CZwpTextInputV3*)> handler) {
    requests.enable = handler;
}

void CZwpTextInputV3::setDisable(F<void(CZwpTextInputV3*)> handler) {
    requests.disable = handler;
}

void CZwpTextInputV3::setSetSurroundingText(F<void(CZwpTextInputV3*, const char*, int32_t, int32_t)> handler) {
    requests.setSurroundingText = handler;
}

void CZwpTextInputV3::setSetTextChangeCause(F<void(CZwpTextInputV3*, zwpTextInputV3ChangeCause)> handler) {
    requests.setTextChangeCause = handler;
}

void CZwpTextInputV3::setSetContentType(F<void(CZwpTextInputV3*, zwpTextInputV3ContentHint, zwpTextInputV3ContentPurpose)> handler) {
    requests.setContentType = handler;
}

void CZwpTextInputV3::setSetCursorRectangle(F<void(CZwpTextInputV3*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.setCursorRectangle = handler;
}

void CZwpTextInputV3::setCommit(F<void(CZwpTextInputV3*)> handler) {
    requests.commit = handler;
}

static void _CZwpTextInputManagerV3Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpTextInputManagerV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpTextInputManagerV3GetTextInput(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* seat) {
    const auto PO = (CZwpTextInputManagerV3*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getTextInput)
        PO->requests.getTextInput(PO, id, seat);
}

static void _CZwpTextInputManagerV3__DestroyListener(wl_listener* l, void* d) {
    CZwpTextInputManagerV3DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpTextInputManagerV3* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpTextInputManagerV3VTable[] = {
    (void*)_CZwpTextInputManagerV3Destroy,
    (void*)_CZwpTextInputManagerV3GetTextInput,
};
static const wl_interface* _CZwpTextInputManagerV3GetTextInputTypes[] = {
    &zwp_text_input_v3_interface,
    &wl_seat_interface,
};

static const wl_message _CZwpTextInputManagerV3Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_text_input", "no", _CZwpTextInputManagerV3GetTextInputTypes + 0},
};

const wl_interface zwp_text_input_manager_v3_interface = {
    "zwp_text_input_manager_v3", 1,
    2, _CZwpTextInputManagerV3Requests,
    0, nullptr,
};

CZwpTextInputManagerV3::CZwpTextInputManagerV3(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_text_input_manager_v3_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpTextInputManagerV3__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpTextInputManagerV3VTable, this, nullptr);
}

CZwpTextInputManagerV3::~CZwpTextInputManagerV3() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpTextInputManagerV3::onDestroyCalled() {
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

void CZwpTextInputManagerV3::setDestroy(F<void(CZwpTextInputManagerV3*)> handler) {
    requests.destroy = handler;
}

void CZwpTextInputManagerV3::setGetTextInput(F<void(CZwpTextInputManagerV3*, uint32_t, wl_resource*)> handler) {
    requests.getTextInput = handler;
}

#undef F
