// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// ext_idle_notify_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2015 Martin Gräßlin
    Copyright © 2022 Simon Ser

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
#include "ext-idle-notify-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface ext_idle_notifier_v1_interface;
extern const wl_interface ext_idle_notification_v1_interface;
extern const wl_interface wl_seat_interface;

static void _CExtIdleNotifierV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtIdleNotifierV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtIdleNotifierV1GetIdleNotification(wl_client* client, wl_resource* resource, uint32_t id, uint32_t timeout, wl_resource* seat) {
    const auto PO = (CExtIdleNotifierV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getIdleNotification)
        PO->requests.getIdleNotification(PO, id, timeout, seat);
}

static void _CExtIdleNotifierV1GetInputIdleNotification(wl_client* client, wl_resource* resource, uint32_t id, uint32_t timeout, wl_resource* seat) {
    const auto PO = (CExtIdleNotifierV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getInputIdleNotification)
        PO->requests.getInputIdleNotification(PO, id, timeout, seat);
}

static void _CExtIdleNotifierV1__DestroyListener(wl_listener* l, void* d) {
    CExtIdleNotifierV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtIdleNotifierV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtIdleNotifierV1VTable[] = {
    (void*)_CExtIdleNotifierV1Destroy,
    (void*)_CExtIdleNotifierV1GetIdleNotification,
    (void*)_CExtIdleNotifierV1GetInputIdleNotification,
};
static const wl_interface* _CExtIdleNotifierV1GetIdleNotificationTypes[] = {
    &ext_idle_notification_v1_interface,
    nullptr,
    &wl_seat_interface,
};
static const wl_interface* _CExtIdleNotifierV1GetInputIdleNotificationTypes[] = {
    &ext_idle_notification_v1_interface,
    nullptr,
    &wl_seat_interface,
};

static const wl_message _CExtIdleNotifierV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_idle_notification", "nuo", _CExtIdleNotifierV1GetIdleNotificationTypes + 0},
    { "get_input_idle_notification", "2nuo", _CExtIdleNotifierV1GetInputIdleNotificationTypes + 0},
};

const wl_interface ext_idle_notifier_v1_interface = {
    "ext_idle_notifier_v1", 2,
    3, _CExtIdleNotifierV1Requests,
    0, nullptr,
};

CExtIdleNotifierV1::CExtIdleNotifierV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_idle_notifier_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtIdleNotifierV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtIdleNotifierV1VTable, this, nullptr);
}

CExtIdleNotifierV1::~CExtIdleNotifierV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtIdleNotifierV1::onDestroyCalled() {
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

void CExtIdleNotifierV1::setDestroy(F<void(CExtIdleNotifierV1*)> handler) {
    requests.destroy = handler;
}

void CExtIdleNotifierV1::setGetIdleNotification(F<void(CExtIdleNotifierV1*, uint32_t, uint32_t, wl_resource*)> handler) {
    requests.getIdleNotification = handler;
}

void CExtIdleNotifierV1::setGetInputIdleNotification(F<void(CExtIdleNotifierV1*, uint32_t, uint32_t, wl_resource*)> handler) {
    requests.getInputIdleNotification = handler;
}

static void _CExtIdleNotificationV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CExtIdleNotificationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CExtIdleNotificationV1__DestroyListener(wl_listener* l, void* d) {
    CExtIdleNotificationV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CExtIdleNotificationV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CExtIdleNotificationV1VTable[] = {
    (void*)_CExtIdleNotificationV1Destroy,
};

void CExtIdleNotificationV1::sendIdled() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CExtIdleNotificationV1::sendResumed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CExtIdleNotificationV1::sendIdledRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CExtIdleNotificationV1::sendResumedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

static const wl_message _CExtIdleNotificationV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CExtIdleNotificationV1Events[] = {
    { "idled", "", dummyTypes + 0},
    { "resumed", "", dummyTypes + 0},
};

const wl_interface ext_idle_notification_v1_interface = {
    "ext_idle_notification_v1", 2,
    1, _CExtIdleNotificationV1Requests,
    2, _CExtIdleNotificationV1Events,
};

CExtIdleNotificationV1::CExtIdleNotificationV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &ext_idle_notification_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CExtIdleNotificationV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CExtIdleNotificationV1VTable, this, nullptr);
}

CExtIdleNotificationV1::~CExtIdleNotificationV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CExtIdleNotificationV1::onDestroyCalled() {
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

void CExtIdleNotificationV1::setDestroy(F<void(CExtIdleNotificationV1*)> handler) {
    requests.destroy = handler;
}

#undef F
