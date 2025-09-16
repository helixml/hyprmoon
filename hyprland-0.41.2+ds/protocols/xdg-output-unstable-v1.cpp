// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// xdg_output_unstable_v1

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
#include "xdg-output-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zxdg_output_manager_v1_interface;
extern const wl_interface zxdg_output_v1_interface;
extern const wl_interface wl_output_interface;

static void _CZxdgOutputManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZxdgOutputManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZxdgOutputManagerV1GetXdgOutput(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* output) {
    const auto PO = (CZxdgOutputManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getXdgOutput)
        PO->requests.getXdgOutput(PO, id, output);
}

static void _CZxdgOutputManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZxdgOutputManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZxdgOutputManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZxdgOutputManagerV1VTable[] = {
    (void*)_CZxdgOutputManagerV1Destroy,
    (void*)_CZxdgOutputManagerV1GetXdgOutput,
};
static const wl_interface* _CZxdgOutputManagerV1GetXdgOutputTypes[] = {
    &zxdg_output_v1_interface,
    &wl_output_interface,
};

static const wl_message _CZxdgOutputManagerV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_xdg_output", "no", _CZxdgOutputManagerV1GetXdgOutputTypes + 0},
};

const wl_interface zxdg_output_manager_v1_interface = {
    "zxdg_output_manager_v1", 3,
    2, _CZxdgOutputManagerV1Requests,
    0, nullptr,
};

CZxdgOutputManagerV1::CZxdgOutputManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zxdg_output_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZxdgOutputManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZxdgOutputManagerV1VTable, this, nullptr);
}

CZxdgOutputManagerV1::~CZxdgOutputManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZxdgOutputManagerV1::onDestroyCalled() {
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

void CZxdgOutputManagerV1::setDestroy(F<void(CZxdgOutputManagerV1*)> handler) {
    requests.destroy = handler;
}

void CZxdgOutputManagerV1::setGetXdgOutput(F<void(CZxdgOutputManagerV1*, uint32_t, wl_resource*)> handler) {
    requests.getXdgOutput = handler;
}

static void _CZxdgOutputV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZxdgOutputV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZxdgOutputV1__DestroyListener(wl_listener* l, void* d) {
    CZxdgOutputV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZxdgOutputV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZxdgOutputV1VTable[] = {
    (void*)_CZxdgOutputV1Destroy,
};

void CZxdgOutputV1::sendLogicalPosition(int32_t x, int32_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y);
}

void CZxdgOutputV1::sendLogicalSize(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, width, height);
}

void CZxdgOutputV1::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZxdgOutputV1::sendName(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, name);
}

void CZxdgOutputV1::sendDescription(const char* description) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, description);
}

void CZxdgOutputV1::sendLogicalPositionRaw(int32_t x, int32_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y);
}

void CZxdgOutputV1::sendLogicalSizeRaw(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, width, height);
}

void CZxdgOutputV1::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZxdgOutputV1::sendNameRaw(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, name);
}

void CZxdgOutputV1::sendDescriptionRaw(const char* description) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, description);
}
static const wl_interface* _CZxdgOutputV1LogicalPositionTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZxdgOutputV1LogicalSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZxdgOutputV1NameTypes[] = {
    nullptr,
};
static const wl_interface* _CZxdgOutputV1DescriptionTypes[] = {
    nullptr,
};

static const wl_message _CZxdgOutputV1Requests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZxdgOutputV1Events[] = {
    { "logical_position", "ii", _CZxdgOutputV1LogicalPositionTypes + 0},
    { "logical_size", "ii", _CZxdgOutputV1LogicalSizeTypes + 0},
    { "done", "", dummyTypes + 0},
    { "name", "2s", _CZxdgOutputV1NameTypes + 0},
    { "description", "2s", _CZxdgOutputV1DescriptionTypes + 0},
};

const wl_interface zxdg_output_v1_interface = {
    "zxdg_output_v1", 3,
    1, _CZxdgOutputV1Requests,
    5, _CZxdgOutputV1Events,
};

CZxdgOutputV1::CZxdgOutputV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zxdg_output_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZxdgOutputV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZxdgOutputV1VTable, this, nullptr);
}

CZxdgOutputV1::~CZxdgOutputV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZxdgOutputV1::onDestroyCalled() {
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

void CZxdgOutputV1::setDestroy(F<void(CZxdgOutputV1*)> handler) {
    requests.destroy = handler;
}

#undef F
