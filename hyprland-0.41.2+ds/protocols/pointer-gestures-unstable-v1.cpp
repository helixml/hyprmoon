// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// pointer_gestures_unstable_v1

/*
 This protocol's authors' copyright notice is:


*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "pointer-gestures-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwp_pointer_gestures_v1_interface;
extern const wl_interface zwp_pointer_gesture_swipe_v1_interface;
extern const wl_interface zwp_pointer_gesture_pinch_v1_interface;
extern const wl_interface zwp_pointer_gesture_hold_v1_interface;
extern const wl_interface wl_pointer_interface;
extern const wl_interface wl_surface_interface;

static void _CZwpPointerGesturesV1GetSwipeGesture(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* pointer) {
    const auto PO = (CZwpPointerGesturesV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getSwipeGesture)
        PO->requests.getSwipeGesture(PO, id, pointer);
}

static void _CZwpPointerGesturesV1GetPinchGesture(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* pointer) {
    const auto PO = (CZwpPointerGesturesV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getPinchGesture)
        PO->requests.getPinchGesture(PO, id, pointer);
}

static void _CZwpPointerGesturesV1Release(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpPointerGesturesV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CZwpPointerGesturesV1GetHoldGesture(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* pointer) {
    const auto PO = (CZwpPointerGesturesV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getHoldGesture)
        PO->requests.getHoldGesture(PO, id, pointer);
}

static void _CZwpPointerGesturesV1__DestroyListener(wl_listener* l, void* d) {
    CZwpPointerGesturesV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpPointerGesturesV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpPointerGesturesV1VTable[] = {
    (void*)_CZwpPointerGesturesV1GetSwipeGesture,
    (void*)_CZwpPointerGesturesV1GetPinchGesture,
    (void*)_CZwpPointerGesturesV1Release,
    (void*)_CZwpPointerGesturesV1GetHoldGesture,
};
static const wl_interface* _CZwpPointerGesturesV1GetSwipeGestureTypes[] = {
    &zwp_pointer_gesture_swipe_v1_interface,
    &wl_pointer_interface,
};
static const wl_interface* _CZwpPointerGesturesV1GetPinchGestureTypes[] = {
    &zwp_pointer_gesture_pinch_v1_interface,
    &wl_pointer_interface,
};
static const wl_interface* _CZwpPointerGesturesV1GetHoldGestureTypes[] = {
    &zwp_pointer_gesture_hold_v1_interface,
    &wl_pointer_interface,
};

static const wl_message _CZwpPointerGesturesV1Requests[] = {
    { "get_swipe_gesture", "no", _CZwpPointerGesturesV1GetSwipeGestureTypes + 0},
    { "get_pinch_gesture", "no", _CZwpPointerGesturesV1GetPinchGestureTypes + 0},
    { "release", "2", dummyTypes + 0},
    { "get_hold_gesture", "3no", _CZwpPointerGesturesV1GetHoldGestureTypes + 0},
};

const wl_interface zwp_pointer_gestures_v1_interface = {
    "zwp_pointer_gestures_v1", 3,
    4, _CZwpPointerGesturesV1Requests,
    0, nullptr,
};

CZwpPointerGesturesV1::CZwpPointerGesturesV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_pointer_gestures_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpPointerGesturesV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpPointerGesturesV1VTable, this, nullptr);
}

CZwpPointerGesturesV1::~CZwpPointerGesturesV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpPointerGesturesV1::onDestroyCalled() {
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

void CZwpPointerGesturesV1::setGetSwipeGesture(F<void(CZwpPointerGesturesV1*, uint32_t, wl_resource*)> handler) {
    requests.getSwipeGesture = handler;
}

void CZwpPointerGesturesV1::setGetPinchGesture(F<void(CZwpPointerGesturesV1*, uint32_t, wl_resource*)> handler) {
    requests.getPinchGesture = handler;
}

void CZwpPointerGesturesV1::setRelease(F<void(CZwpPointerGesturesV1*)> handler) {
    requests.release = handler;
}

void CZwpPointerGesturesV1::setGetHoldGesture(F<void(CZwpPointerGesturesV1*, uint32_t, wl_resource*)> handler) {
    requests.getHoldGesture = handler;
}

static void _CZwpPointerGestureSwipeV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpPointerGestureSwipeV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpPointerGestureSwipeV1__DestroyListener(wl_listener* l, void* d) {
    CZwpPointerGestureSwipeV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpPointerGestureSwipeV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpPointerGestureSwipeV1VTable[] = {
    (void*)_CZwpPointerGestureSwipeV1Destroy,
};

void CZwpPointerGestureSwipeV1::sendBegin(uint32_t serial, uint32_t time, wl_resource* surface, uint32_t fingers) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, fingers);
}

void CZwpPointerGestureSwipeV1::sendUpdate(uint32_t time, wl_fixed_t dx, wl_fixed_t dy) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, time, dx, dy);
}

void CZwpPointerGestureSwipeV1::sendEnd(uint32_t serial, uint32_t time, int32_t cancelled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, time, cancelled);
}

void CZwpPointerGestureSwipeV1::sendBeginRaw(uint32_t serial, uint32_t time, wl_resource* surface, uint32_t fingers) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, fingers);
}

void CZwpPointerGestureSwipeV1::sendUpdateRaw(uint32_t time, wl_fixed_t dx, wl_fixed_t dy) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, time, dx, dy);
}

void CZwpPointerGestureSwipeV1::sendEndRaw(uint32_t serial, uint32_t time, int32_t cancelled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, time, cancelled);
}
static const wl_interface* _CZwpPointerGestureSwipeV1BeginTypes[] = {
    nullptr,
    nullptr,
    &wl_surface_interface,
    nullptr,
};
static const wl_interface* _CZwpPointerGestureSwipeV1UpdateTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpPointerGestureSwipeV1EndTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwpPointerGestureSwipeV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpPointerGestureSwipeV1Events[] = {
    { "begin", "uuou", _CZwpPointerGestureSwipeV1BeginTypes + 0},
    { "update", "uff", _CZwpPointerGestureSwipeV1UpdateTypes + 0},
    { "end", "uui", _CZwpPointerGestureSwipeV1EndTypes + 0},
};

const wl_interface zwp_pointer_gesture_swipe_v1_interface = {
    "zwp_pointer_gesture_swipe_v1", 2,
    1, _CZwpPointerGestureSwipeV1Requests,
    3, _CZwpPointerGestureSwipeV1Events,
};

CZwpPointerGestureSwipeV1::CZwpPointerGestureSwipeV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_pointer_gesture_swipe_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpPointerGestureSwipeV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpPointerGestureSwipeV1VTable, this, nullptr);
}

CZwpPointerGestureSwipeV1::~CZwpPointerGestureSwipeV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpPointerGestureSwipeV1::onDestroyCalled() {
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

void CZwpPointerGestureSwipeV1::setDestroy(F<void(CZwpPointerGestureSwipeV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwpPointerGesturePinchV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpPointerGesturePinchV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpPointerGesturePinchV1__DestroyListener(wl_listener* l, void* d) {
    CZwpPointerGesturePinchV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpPointerGesturePinchV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpPointerGesturePinchV1VTable[] = {
    (void*)_CZwpPointerGesturePinchV1Destroy,
};

void CZwpPointerGesturePinchV1::sendBegin(uint32_t serial, uint32_t time, wl_resource* surface, uint32_t fingers) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, fingers);
}

void CZwpPointerGesturePinchV1::sendUpdate(uint32_t time, wl_fixed_t dx, wl_fixed_t dy, wl_fixed_t scale, wl_fixed_t rotation) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, time, dx, dy, scale, rotation);
}

void CZwpPointerGesturePinchV1::sendEnd(uint32_t serial, uint32_t time, int32_t cancelled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, time, cancelled);
}

void CZwpPointerGesturePinchV1::sendBeginRaw(uint32_t serial, uint32_t time, wl_resource* surface, uint32_t fingers) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, fingers);
}

void CZwpPointerGesturePinchV1::sendUpdateRaw(uint32_t time, wl_fixed_t dx, wl_fixed_t dy, wl_fixed_t scale, wl_fixed_t rotation) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, time, dx, dy, scale, rotation);
}

void CZwpPointerGesturePinchV1::sendEndRaw(uint32_t serial, uint32_t time, int32_t cancelled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, time, cancelled);
}
static const wl_interface* _CZwpPointerGesturePinchV1BeginTypes[] = {
    nullptr,
    nullptr,
    &wl_surface_interface,
    nullptr,
};
static const wl_interface* _CZwpPointerGesturePinchV1UpdateTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwpPointerGesturePinchV1EndTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwpPointerGesturePinchV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwpPointerGesturePinchV1Events[] = {
    { "begin", "uuou", _CZwpPointerGesturePinchV1BeginTypes + 0},
    { "update", "uffff", _CZwpPointerGesturePinchV1UpdateTypes + 0},
    { "end", "uui", _CZwpPointerGesturePinchV1EndTypes + 0},
};

const wl_interface zwp_pointer_gesture_pinch_v1_interface = {
    "zwp_pointer_gesture_pinch_v1", 2,
    1, _CZwpPointerGesturePinchV1Requests,
    3, _CZwpPointerGesturePinchV1Events,
};

CZwpPointerGesturePinchV1::CZwpPointerGesturePinchV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_pointer_gesture_pinch_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpPointerGesturePinchV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpPointerGesturePinchV1VTable, this, nullptr);
}

CZwpPointerGesturePinchV1::~CZwpPointerGesturePinchV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpPointerGesturePinchV1::onDestroyCalled() {
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

void CZwpPointerGesturePinchV1::setDestroy(F<void(CZwpPointerGesturePinchV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwpPointerGestureHoldV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwpPointerGestureHoldV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwpPointerGestureHoldV1__DestroyListener(wl_listener* l, void* d) {
    CZwpPointerGestureHoldV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwpPointerGestureHoldV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwpPointerGestureHoldV1VTable[] = {
    (void*)_CZwpPointerGestureHoldV1Destroy,
};

void CZwpPointerGestureHoldV1::sendBegin(uint32_t serial, uint32_t time, wl_resource* surface, uint32_t fingers) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, fingers);
}

void CZwpPointerGestureHoldV1::sendEnd(uint32_t serial, uint32_t time, int32_t cancelled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, time, cancelled);
}

void CZwpPointerGestureHoldV1::sendBeginRaw(uint32_t serial, uint32_t time, wl_resource* surface, uint32_t fingers) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, fingers);
}

void CZwpPointerGestureHoldV1::sendEndRaw(uint32_t serial, uint32_t time, int32_t cancelled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, time, cancelled);
}
static const wl_interface* _CZwpPointerGestureHoldV1BeginTypes[] = {
    nullptr,
    nullptr,
    &wl_surface_interface,
    nullptr,
};
static const wl_interface* _CZwpPointerGestureHoldV1EndTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CZwpPointerGestureHoldV1Requests[] = {
    { "destroy", "3", dummyTypes + 0},
};

static const wl_message _CZwpPointerGestureHoldV1Events[] = {
    { "begin", "3uuou", _CZwpPointerGestureHoldV1BeginTypes + 0},
    { "end", "3uui", _CZwpPointerGestureHoldV1EndTypes + 0},
};

const wl_interface zwp_pointer_gesture_hold_v1_interface = {
    "zwp_pointer_gesture_hold_v1", 3,
    1, _CZwpPointerGestureHoldV1Requests,
    2, _CZwpPointerGestureHoldV1Events,
};

CZwpPointerGestureHoldV1::CZwpPointerGestureHoldV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwp_pointer_gesture_hold_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwpPointerGestureHoldV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwpPointerGestureHoldV1VTable, this, nullptr);
}

CZwpPointerGestureHoldV1::~CZwpPointerGestureHoldV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwpPointerGestureHoldV1::onDestroyCalled() {
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

void CZwpPointerGestureHoldV1::setDestroy(F<void(CZwpPointerGestureHoldV1*)> handler) {
    requests.destroy = handler;
}

#undef F
