// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// xdg_activation_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2020 Aleix Pol Gonzalez <aleixpol@kde.org>
    Copyright © 2020 Carlos Garnacho <carlosg@gnome.org>

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
#include "xdg-activation-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface xdg_activation_v1_interface;
extern const wl_interface xdg_activation_token_v1_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_seat_interface;

static void _CXdgActivationV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgActivationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgActivationV1GetActivationToken(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CXdgActivationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getActivationToken)
        PO->requests.getActivationToken(PO, id);
}

static void _CXdgActivationV1Activate(wl_client* client, wl_resource* resource, const char* token, wl_resource* surface) {
    const auto PO = (CXdgActivationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.activate)
        PO->requests.activate(PO, token, surface);
}

static void _CXdgActivationV1__DestroyListener(wl_listener* l, void* d) {
    CXdgActivationV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgActivationV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgActivationV1VTable[] = {
    (void*)_CXdgActivationV1Destroy,
    (void*)_CXdgActivationV1GetActivationToken,
    (void*)_CXdgActivationV1Activate,
};
static const wl_interface* _CXdgActivationV1GetActivationTokenTypes[] = {
    &xdg_activation_token_v1_interface,
};
static const wl_interface* _CXdgActivationV1ActivateTypes[] = {
    nullptr,
    &wl_surface_interface,
};

static const wl_message _CXdgActivationV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_activation_token", "n", _CXdgActivationV1GetActivationTokenTypes + 0},
    { "activate", "so", _CXdgActivationV1ActivateTypes + 0},
};

const wl_interface xdg_activation_v1_interface = {
    "xdg_activation_v1", 1,
    3, _CXdgActivationV1Requests,
    0, nullptr,
};

CXdgActivationV1::CXdgActivationV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_activation_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgActivationV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgActivationV1VTable, this, nullptr);
}

CXdgActivationV1::~CXdgActivationV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgActivationV1::onDestroyCalled() {
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

void CXdgActivationV1::setDestroy(F<void(CXdgActivationV1*)> handler) {
    requests.destroy = handler;
}

void CXdgActivationV1::setGetActivationToken(F<void(CXdgActivationV1*, uint32_t)> handler) {
    requests.getActivationToken = handler;
}

void CXdgActivationV1::setActivate(F<void(CXdgActivationV1*, const char*, wl_resource*)> handler) {
    requests.activate = handler;
}

static void _CXdgActivationTokenV1SetSerial(wl_client* client, wl_resource* resource, uint32_t serial, wl_resource* seat) {
    const auto PO = (CXdgActivationTokenV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSerial)
        PO->requests.setSerial(PO, serial, seat);
}

static void _CXdgActivationTokenV1SetAppId(wl_client* client, wl_resource* resource, const char* app_id) {
    const auto PO = (CXdgActivationTokenV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setAppId)
        PO->requests.setAppId(PO, app_id);
}

static void _CXdgActivationTokenV1SetSurface(wl_client* client, wl_resource* resource, wl_resource* surface) {
    const auto PO = (CXdgActivationTokenV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSurface)
        PO->requests.setSurface(PO, surface);
}

static void _CXdgActivationTokenV1Commit(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgActivationTokenV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.commit)
        PO->requests.commit(PO);
}

static void _CXdgActivationTokenV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CXdgActivationTokenV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CXdgActivationTokenV1__DestroyListener(wl_listener* l, void* d) {
    CXdgActivationTokenV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CXdgActivationTokenV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CXdgActivationTokenV1VTable[] = {
    (void*)_CXdgActivationTokenV1SetSerial,
    (void*)_CXdgActivationTokenV1SetAppId,
    (void*)_CXdgActivationTokenV1SetSurface,
    (void*)_CXdgActivationTokenV1Commit,
    (void*)_CXdgActivationTokenV1Destroy,
};

void CXdgActivationTokenV1::sendDone(const char* token) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, token);
}

void CXdgActivationTokenV1::sendDoneRaw(const char* token) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, token);
}
static const wl_interface* _CXdgActivationTokenV1SetSerialTypes[] = {
    nullptr,
    &wl_seat_interface,
};
static const wl_interface* _CXdgActivationTokenV1SetAppIdTypes[] = {
    nullptr,
};
static const wl_interface* _CXdgActivationTokenV1SetSurfaceTypes[] = {
    &wl_surface_interface,
};
static const wl_interface* _CXdgActivationTokenV1DoneTypes[] = {
    nullptr,
};

static const wl_message _CXdgActivationTokenV1Requests[] = {
    { "set_serial", "uo", _CXdgActivationTokenV1SetSerialTypes + 0},
    { "set_app_id", "s", _CXdgActivationTokenV1SetAppIdTypes + 0},
    { "set_surface", "o", _CXdgActivationTokenV1SetSurfaceTypes + 0},
    { "commit", "", dummyTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CXdgActivationTokenV1Events[] = {
    { "done", "s", _CXdgActivationTokenV1DoneTypes + 0},
};

const wl_interface xdg_activation_token_v1_interface = {
    "xdg_activation_token_v1", 1,
    5, _CXdgActivationTokenV1Requests,
    1, _CXdgActivationTokenV1Events,
};

CXdgActivationTokenV1::CXdgActivationTokenV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &xdg_activation_token_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CXdgActivationTokenV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CXdgActivationTokenV1VTable, this, nullptr);
}

CXdgActivationTokenV1::~CXdgActivationTokenV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CXdgActivationTokenV1::onDestroyCalled() {
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

void CXdgActivationTokenV1::setSetSerial(F<void(CXdgActivationTokenV1*, uint32_t, wl_resource*)> handler) {
    requests.setSerial = handler;
}

void CXdgActivationTokenV1::setSetAppId(F<void(CXdgActivationTokenV1*, const char*)> handler) {
    requests.setAppId = handler;
}

void CXdgActivationTokenV1::setSetSurface(F<void(CXdgActivationTokenV1*, wl_resource*)> handler) {
    requests.setSurface = handler;
}

void CXdgActivationTokenV1::setCommit(F<void(CXdgActivationTokenV1*)> handler) {
    requests.commit = handler;
}

void CXdgActivationTokenV1::setDestroy(F<void(CXdgActivationTokenV1*)> handler) {
    requests.destroy = handler;
}

#undef F
