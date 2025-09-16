// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// ext_session_lock_v1

/*
 This protocol's authors' copyright notice is:


    Copyright 2021 Isaac Freund

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "ext-session-lock-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface ext_session_lock_manager_v1_interface;
extern const wl_interface ext_session_lock_v1_interface;
extern const wl_interface ext_session_lock_surface_v1_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_output_interface;

static void _CExtSessionLockManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtSessionLockManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtSessionLockManagerV1Lock(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CExtSessionLockManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.lock)
        PO->requests.lock(PO, id);
}

static void _CExtSessionLockManagerV1__DestroyListener(wl_listener* l, void* d) {
    CExtSessionLockManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtSessionLockManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtSessionLockManagerV1VTable[] = {
    (void*)_CExtSessionLockManagerV1Destroy,
    (void*)_CExtSessionLockManagerV1Lock,
};
static const wl_interface* _CExtSessionLockManagerV1LockTypes[] = {
    &ext_session_lock_v1_interface,
};

static const wl_message _CExtSessionLockManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "lock", "n", _CExtSessionLockManagerV1LockTypes + 0},
};

const wl_interface ext_session_lock_manager_v1_interface = {
    "ext_session_lock_manager_v1", 1,
    2, _CExtSessionLockManagerV1Requests,
    0, nullptr,
};

CExtSessionLockManagerV1::CExtSessionLockManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_session_lock_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtSessionLockManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtSessionLockManagerV1VTable, this, nullptr);
}

CExtSessionLockManagerV1::~CExtSessionLockManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtSessionLockManagerV1::onDestroyCalled() {
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

void CExtSessionLockManagerV1::setDestroy(F<void(CExtSessionLockManagerV1*)> handler) {
    requests.destroy = handler;
}

void CExtSessionLockManagerV1::setLock(F<void(CExtSessionLockManagerV1*, uint32_t)> handler) {
    requests.lock = handler;
}

static void _CExtSessionLockV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtSessionLockV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtSessionLockV1GetLockSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface, wl_resource* output) {
    const auto PO = (CExtSessionLockV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getLockSurface)
        PO->requests.getLockSurface(PO, id, surface, output);
}

static void _CExtSessionLockV1UnlockAndDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtSessionLockV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.unlockAndDestroy)
        PO->requests.unlockAndDestroy(PO);
}

static void _CExtSessionLockV1__DestroyListener(wl_listener* l, void* d) {
    CExtSessionLockV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtSessionLockV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtSessionLockV1VTable[] = {
    (void*)_CExtSessionLockV1Destroy,
    (void*)_CExtSessionLockV1GetLockSurface,
    (void*)_CExtSessionLockV1UnlockAndDestroy,
};

void CExtSessionLockV1::sendLocked() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CExtSessionLockV1::sendFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CExtSessionLockV1::sendLockedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CExtSessionLockV1::sendFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}
static const wl_interface* _CExtSessionLockV1GetLockSurfaceTypes[] = {
    &ext_session_lock_surface_v1_interface,
    &wl_surface_interface,
    &wl_output_interface,
};

static const wl_message _CExtSessionLockV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_lock_surface", "noo", _CExtSessionLockV1GetLockSurfaceTypes + 0},
    { "unlock_and_destroy", "", dummyTypes + 0},
};

static const wl_message _CExtSessionLockV1Events[] = {
    { "locked", "", dummyTypes + 0},
    { "finished", "", dummyTypes + 0},
};

const wl_interface ext_session_lock_v1_interface = {
    "ext_session_lock_v1", 1,
    3, _CExtSessionLockV1Requests,
    2, _CExtSessionLockV1Events,
};

CExtSessionLockV1::CExtSessionLockV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_session_lock_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtSessionLockV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtSessionLockV1VTable, this, nullptr);
}

CExtSessionLockV1::~CExtSessionLockV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtSessionLockV1::onDestroyCalled() {
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

void CExtSessionLockV1::setDestroy(F<void(CExtSessionLockV1*)> handler) {
    requests.destroy = handler;
}

void CExtSessionLockV1::setGetLockSurface(F<void(CExtSessionLockV1*, uint32_t, wl_resource*, wl_resource*)> handler) {
    requests.getLockSurface = handler;
}

void CExtSessionLockV1::setUnlockAndDestroy(F<void(CExtSessionLockV1*)> handler) {
    requests.unlockAndDestroy = handler;
}

static void _CExtSessionLockSurfaceV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtSessionLockSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtSessionLockSurfaceV1AckConfigure(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CExtSessionLockSurfaceV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.ackConfigure)
        PO->requests.ackConfigure(PO, serial);
}

static void _CExtSessionLockSurfaceV1__DestroyListener(wl_listener* l, void* d) {
    CExtSessionLockSurfaceV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtSessionLockSurfaceV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtSessionLockSurfaceV1VTable[] = {
    (void*)_CExtSessionLockSurfaceV1Destroy,
    (void*)_CExtSessionLockSurfaceV1AckConfigure,
};

void CExtSessionLockSurfaceV1::sendConfigure(uint32_t serial, uint32_t width, uint32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, width, height);
}

void CExtSessionLockSurfaceV1::sendConfigureRaw(uint32_t serial, uint32_t width, uint32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, width, height);
}
static const wl_interface* _CExtSessionLockSurfaceV1AckConfigureTypes[] = {
    nullptr,
};
static const wl_interface* _CExtSessionLockSurfaceV1ConfigureTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CExtSessionLockSurfaceV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "ack_configure", "u", _CExtSessionLockSurfaceV1AckConfigureTypes + 0},
};

static const wl_message _CExtSessionLockSurfaceV1Events[] = {
    { "configure", "uuu", _CExtSessionLockSurfaceV1ConfigureTypes + 0},
};

const wl_interface ext_session_lock_surface_v1_interface = {
    "ext_session_lock_surface_v1", 1,
    2, _CExtSessionLockSurfaceV1Requests,
    1, _CExtSessionLockSurfaceV1Events,
};

CExtSessionLockSurfaceV1::CExtSessionLockSurfaceV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_session_lock_surface_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtSessionLockSurfaceV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtSessionLockSurfaceV1VTable, this, nullptr);
}

CExtSessionLockSurfaceV1::~CExtSessionLockSurfaceV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtSessionLockSurfaceV1::onDestroyCalled() {
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

void CExtSessionLockSurfaceV1::setDestroy(F<void(CExtSessionLockSurfaceV1*)> handler) {
    requests.destroy = handler;
}

void CExtSessionLockSurfaceV1::setAckConfigure(F<void(CExtSessionLockSurfaceV1*, uint32_t)> handler) {
    requests.ackConfigure = handler;
}

#undef F
