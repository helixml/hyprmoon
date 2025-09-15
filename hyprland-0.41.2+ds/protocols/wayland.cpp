// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wayland

/*
 This protocol's authors' copyright notice is:


    Copyright © 2008-2011 Kristian Høgsberg
    Copyright © 2010-2011 Intel Corporation
    Copyright © 2012-2013 Collabora, Ltd.

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice (including the
    next paragraph) shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
    BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "wayland.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wl_display_interface;
extern const wl_interface wl_registry_interface;
extern const wl_interface wl_callback_interface;
extern const wl_interface wl_compositor_interface;
extern const wl_interface wl_shm_pool_interface;
extern const wl_interface wl_shm_interface;
extern const wl_interface wl_buffer_interface;
extern const wl_interface wl_data_offer_interface;
extern const wl_interface wl_data_source_interface;
extern const wl_interface wl_data_device_interface;
extern const wl_interface wl_data_device_manager_interface;
extern const wl_interface wl_shell_interface;
extern const wl_interface wl_shell_surface_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_seat_interface;
extern const wl_interface wl_pointer_interface;
extern const wl_interface wl_keyboard_interface;
extern const wl_interface wl_touch_interface;
extern const wl_interface wl_output_interface;
extern const wl_interface wl_region_interface;
extern const wl_interface wl_subcompositor_interface;
extern const wl_interface wl_subsurface_interface;

static void _CWlDisplaySync(wl_client* client, wl_resource* resource, uint32_t callback) {
    const auto PO = (CWlDisplay*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.sync)
        PO->requests.sync(PO, callback);
}

static void _CWlDisplayGetRegistry(wl_client* client, wl_resource* resource, uint32_t registry) {
    const auto PO = (CWlDisplay*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getRegistry)
        PO->requests.getRegistry(PO, registry);
}

static void _CWlDisplay__DestroyListener(wl_listener* l, void* d) {
    CWlDisplayDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlDisplay* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlDisplayVTable[] = {
    (void*)_CWlDisplaySync,
    (void*)_CWlDisplayGetRegistry,
};

void CWlDisplay::sendError(wl_resource* object_id, uint32_t code, const char* message) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, object_id, code, message);
}

void CWlDisplay::sendDeleteId(uint32_t id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, id);
}

void CWlDisplay::sendErrorRaw(wl_resource* object_id, uint32_t code, const char* message) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, object_id, code, message);
}

void CWlDisplay::sendDeleteIdRaw(uint32_t id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, id);
}
static const wl_interface* _CWlDisplaySyncTypes[] = {
    &wl_callback_interface,
};
static const wl_interface* _CWlDisplayGetRegistryTypes[] = {
    &wl_registry_interface,
};
static const wl_interface* _CWlDisplayErrorTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDisplayDeleteIdTypes[] = {
    nullptr,
};

static const wl_message _CWlDisplayRequests[] = {
    { "sync", "n", _CWlDisplaySyncTypes + 0},
    { "get_registry", "n", _CWlDisplayGetRegistryTypes + 0},
};

static const wl_message _CWlDisplayEvents[] = {
    { "error", "ous", _CWlDisplayErrorTypes + 0},
    { "delete_id", "u", _CWlDisplayDeleteIdTypes + 0},
};

const wl_interface wl_display_interface = {
    "wl_display", 1,
    2, _CWlDisplayRequests,
    2, _CWlDisplayEvents,
};

CWlDisplay::CWlDisplay(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_display_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlDisplay__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlDisplayVTable, this, nullptr);
}

CWlDisplay::~CWlDisplay() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlDisplay::onDestroyCalled() {
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

void CWlDisplay::setSync(F<void(CWlDisplay*, uint32_t)> handler) {
    requests.sync = handler;
}

void CWlDisplay::setGetRegistry(F<void(CWlDisplay*, uint32_t)> handler) {
    requests.getRegistry = handler;
}

static void _CWlRegistryBind(wl_client* client, wl_resource* resource, uint32_t name, uint32_t id) {
    const auto PO = (CWlRegistry*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.bind)
        PO->requests.bind(PO, name, id);
}

static void _CWlRegistry__DestroyListener(wl_listener* l, void* d) {
    CWlRegistryDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlRegistry* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlRegistryVTable[] = {
    (void*)_CWlRegistryBind,
};

void CWlRegistry::sendGlobal(uint32_t name, const char* interface, uint32_t version) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name, interface, version);
}

void CWlRegistry::sendGlobalRemove(uint32_t name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, name);
}

void CWlRegistry::sendGlobalRaw(uint32_t name, const char* interface, uint32_t version) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name, interface, version);
}

void CWlRegistry::sendGlobalRemoveRaw(uint32_t name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, name);
}
static const wl_interface* _CWlRegistryBindTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlRegistryGlobalTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlRegistryGlobalRemoveTypes[] = {
    nullptr,
};

static const wl_message _CWlRegistryRequests[] = {
    { "bind", "usun", _CWlRegistryBindTypes + 0},
};

static const wl_message _CWlRegistryEvents[] = {
    { "global", "usu", _CWlRegistryGlobalTypes + 0},
    { "global_remove", "u", _CWlRegistryGlobalRemoveTypes + 0},
};

const wl_interface wl_registry_interface = {
    "wl_registry", 1,
    1, _CWlRegistryRequests,
    2, _CWlRegistryEvents,
};

CWlRegistry::CWlRegistry(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_registry_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlRegistry__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlRegistryVTable, this, nullptr);
}

CWlRegistry::~CWlRegistry() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlRegistry::onDestroyCalled() {
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

void CWlRegistry::setBind(F<void(CWlRegistry*, uint32_t, uint32_t)> handler) {
    requests.bind = handler;
}

static void _CWlCallback__DestroyListener(wl_listener* l, void* d) {
    CWlCallbackDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlCallback* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlCallbackVTable[] = {
};

void CWlCallback::sendDone(uint32_t callback_data) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, callback_data);
}

void CWlCallback::sendDoneRaw(uint32_t callback_data) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, callback_data);
}
static const wl_interface* _CWlCallbackDoneTypes[] = {
    nullptr,
};

static const wl_message _CWlCallbackEvents[] = {
    { "done", "u", _CWlCallbackDoneTypes + 0},
};

const wl_interface wl_callback_interface = {
    "wl_callback", 1,
    0, nullptr,
    1, _CWlCallbackEvents,
};

CWlCallback::CWlCallback(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_callback_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlCallback__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlCallbackVTable, this, nullptr);
}

CWlCallback::~CWlCallback() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlCallback::onDestroyCalled() {
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

static void _CWlCompositorCreateSurface(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlCompositor*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createSurface)
        PO->requests.createSurface(PO, id);
}

static void _CWlCompositorCreateRegion(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlCompositor*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createRegion)
        PO->requests.createRegion(PO, id);
}

static void _CWlCompositor__DestroyListener(wl_listener* l, void* d) {
    CWlCompositorDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlCompositor* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlCompositorVTable[] = {
    (void*)_CWlCompositorCreateSurface,
    (void*)_CWlCompositorCreateRegion,
};
static const wl_interface* _CWlCompositorCreateSurfaceTypes[] = {
    &wl_surface_interface,
};
static const wl_interface* _CWlCompositorCreateRegionTypes[] = {
    &wl_region_interface,
};

static const wl_message _CWlCompositorRequests[] = {
    { "create_surface", "n", _CWlCompositorCreateSurfaceTypes + 0},
    { "create_region", "n", _CWlCompositorCreateRegionTypes + 0},
};

const wl_interface wl_compositor_interface = {
    "wl_compositor", 6,
    2, _CWlCompositorRequests,
    0, nullptr,
};

CWlCompositor::CWlCompositor(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_compositor_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlCompositor__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlCompositorVTable, this, nullptr);
}

CWlCompositor::~CWlCompositor() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlCompositor::onDestroyCalled() {
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

void CWlCompositor::setCreateSurface(F<void(CWlCompositor*, uint32_t)> handler) {
    requests.createSurface = handler;
}

void CWlCompositor::setCreateRegion(F<void(CWlCompositor*, uint32_t)> handler) {
    requests.createRegion = handler;
}

static void _CWlShmPoolCreateBuffer(wl_client* client, wl_resource* resource, uint32_t id, int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format) {
    const auto PO = (CWlShmPool*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createBuffer)
        PO->requests.createBuffer(PO, id, offset, width, height, stride, format);
}

static void _CWlShmPoolDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlShmPool*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlShmPoolResize(wl_client* client, wl_resource* resource, int32_t size) {
    const auto PO = (CWlShmPool*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.resize)
        PO->requests.resize(PO, size);
}

static void _CWlShmPool__DestroyListener(wl_listener* l, void* d) {
    CWlShmPoolDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlShmPool* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlShmPoolVTable[] = {
    (void*)_CWlShmPoolCreateBuffer,
    (void*)_CWlShmPoolDestroy,
    (void*)_CWlShmPoolResize,
};
static const wl_interface* _CWlShmPoolCreateBufferTypes[] = {
    &wl_buffer_interface,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlShmPoolResizeTypes[] = {
    nullptr,
};

static const wl_message _CWlShmPoolRequests[] = {
    { "create_buffer", "niiiiu", _CWlShmPoolCreateBufferTypes + 0},
    { "destroy", "", dummyTypes + 0},
    { "resize", "i", _CWlShmPoolResizeTypes + 0},
};

const wl_interface wl_shm_pool_interface = {
    "wl_shm_pool", 2,
    3, _CWlShmPoolRequests,
    0, nullptr,
};

CWlShmPool::CWlShmPool(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_shm_pool_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlShmPool__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlShmPoolVTable, this, nullptr);
}

CWlShmPool::~CWlShmPool() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlShmPool::onDestroyCalled() {
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

void CWlShmPool::setCreateBuffer(F<void(CWlShmPool*, uint32_t, int32_t, int32_t, int32_t, int32_t, uint32_t)> handler) {
    requests.createBuffer = handler;
}

void CWlShmPool::setDestroy(F<void(CWlShmPool*)> handler) {
    requests.destroy = handler;
}

void CWlShmPool::setResize(F<void(CWlShmPool*, int32_t)> handler) {
    requests.resize = handler;
}

static void _CWlShmCreatePool(wl_client* client, wl_resource* resource, uint32_t id, int32_t fd, int32_t size) {
    const auto PO = (CWlShm*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createPool)
        PO->requests.createPool(PO, id, fd, size);
}

static void _CWlShmRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlShm*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlShm__DestroyListener(wl_listener* l, void* d) {
    CWlShmDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlShm* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlShmVTable[] = {
    (void*)_CWlShmCreatePool,
    (void*)_CWlShmRelease,
};

void CWlShm::sendFormat(enum wl_shm_format format) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, format);
}

void CWlShm::sendFormatRaw(enum wl_shm_format format) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, format);
}
static const wl_interface* _CWlShmCreatePoolTypes[] = {
    &wl_shm_pool_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlShmFormatTypes[] = {
    nullptr,
};

static const wl_message _CWlShmRequests[] = {
    { "create_pool", "nhi", _CWlShmCreatePoolTypes + 0},
    { "release", "2", dummyTypes + 0},
};

static const wl_message _CWlShmEvents[] = {
    { "format", "u", _CWlShmFormatTypes + 0},
};

const wl_interface wl_shm_interface = {
    "wl_shm", 2,
    2, _CWlShmRequests,
    1, _CWlShmEvents,
};

CWlShm::CWlShm(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_shm_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlShm__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlShmVTable, this, nullptr);
}

CWlShm::~CWlShm() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlShm::onDestroyCalled() {
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

void CWlShm::setCreatePool(F<void(CWlShm*, uint32_t, int32_t, int32_t)> handler) {
    requests.createPool = handler;
}

void CWlShm::setRelease(F<void(CWlShm*)> handler) {
    requests.release = handler;
}

static void _CWlBufferDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlBuffer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlBuffer__DestroyListener(wl_listener* l, void* d) {
    CWlBufferDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlBuffer* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlBufferVTable[] = {
    (void*)_CWlBufferDestroy,
};

void CWlBuffer::sendRelease() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CWlBuffer::sendReleaseRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

static const wl_message _CWlBufferRequests[] = {
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CWlBufferEvents[] = {
    { "release", "", dummyTypes + 0},
};

const wl_interface wl_buffer_interface = {
    "wl_buffer", 1,
    1, _CWlBufferRequests,
    1, _CWlBufferEvents,
};

CWlBuffer::CWlBuffer(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_buffer_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlBuffer__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlBufferVTable, this, nullptr);
}

CWlBuffer::~CWlBuffer() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlBuffer::onDestroyCalled() {
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

void CWlBuffer::setDestroy(F<void(CWlBuffer*)> handler) {
    requests.destroy = handler;
}

static void _CWlDataOfferAccept(wl_client* client, wl_resource* resource, uint32_t serial, const char* mime_type) {
    const auto PO = (CWlDataOffer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.accept)
        PO->requests.accept(PO, serial, mime_type);
}

static void _CWlDataOfferReceive(wl_client* client, wl_resource* resource, const char* mime_type, int32_t fd) {
    const auto PO = (CWlDataOffer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.receive)
        PO->requests.receive(PO, mime_type, fd);
}

static void _CWlDataOfferDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlDataOffer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlDataOfferFinish(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlDataOffer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.finish)
        PO->requests.finish(PO);
}

static void _CWlDataOfferSetActions(wl_client* client, wl_resource* resource, uint32_t dnd_actions, uint32_t preferred_action) {
    const auto PO = (CWlDataOffer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setActions)
        PO->requests.setActions(PO, dnd_actions, preferred_action);
}

static void _CWlDataOffer__DestroyListener(wl_listener* l, void* d) {
    CWlDataOfferDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlDataOffer* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlDataOfferVTable[] = {
    (void*)_CWlDataOfferAccept,
    (void*)_CWlDataOfferReceive,
    (void*)_CWlDataOfferDestroy,
    (void*)_CWlDataOfferFinish,
    (void*)_CWlDataOfferSetActions,
};

void CWlDataOffer::sendOffer(const char* mime_type) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mime_type);
}

void CWlDataOffer::sendSourceActions(uint32_t source_actions) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, source_actions);
}

void CWlDataOffer::sendAction(uint32_t dnd_action) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, dnd_action);
}

void CWlDataOffer::sendOfferRaw(const char* mime_type) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mime_type);
}

void CWlDataOffer::sendSourceActionsRaw(uint32_t source_actions) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, source_actions);
}

void CWlDataOffer::sendActionRaw(uint32_t dnd_action) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, dnd_action);
}
static const wl_interface* _CWlDataOfferAcceptTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDataOfferReceiveTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDataOfferSetActionsTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDataOfferOfferTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDataOfferSourceActionsTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDataOfferActionTypes[] = {
    nullptr,
};

static const wl_message _CWlDataOfferRequests[] = {
    { "accept", "u?s", _CWlDataOfferAcceptTypes + 0},
    { "receive", "sh", _CWlDataOfferReceiveTypes + 0},
    { "destroy", "", dummyTypes + 0},
    { "finish", "3", dummyTypes + 0},
    { "set_actions", "3uu", _CWlDataOfferSetActionsTypes + 0},
};

static const wl_message _CWlDataOfferEvents[] = {
    { "offer", "s", _CWlDataOfferOfferTypes + 0},
    { "source_actions", "3u", _CWlDataOfferSourceActionsTypes + 0},
    { "action", "3u", _CWlDataOfferActionTypes + 0},
};

const wl_interface wl_data_offer_interface = {
    "wl_data_offer", 3,
    5, _CWlDataOfferRequests,
    3, _CWlDataOfferEvents,
};

CWlDataOffer::CWlDataOffer(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_data_offer_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlDataOffer__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlDataOfferVTable, this, nullptr);
}

CWlDataOffer::~CWlDataOffer() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlDataOffer::onDestroyCalled() {
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

void CWlDataOffer::setAccept(F<void(CWlDataOffer*, uint32_t, const char*)> handler) {
    requests.accept = handler;
}

void CWlDataOffer::setReceive(F<void(CWlDataOffer*, const char*, int32_t)> handler) {
    requests.receive = handler;
}

void CWlDataOffer::setDestroy(F<void(CWlDataOffer*)> handler) {
    requests.destroy = handler;
}

void CWlDataOffer::setFinish(F<void(CWlDataOffer*)> handler) {
    requests.finish = handler;
}

void CWlDataOffer::setSetActions(F<void(CWlDataOffer*, uint32_t, uint32_t)> handler) {
    requests.setActions = handler;
}

static void _CWlDataSourceOffer(wl_client* client, wl_resource* resource, const char* mime_type) {
    const auto PO = (CWlDataSource*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.offer)
        PO->requests.offer(PO, mime_type);
}

static void _CWlDataSourceDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlDataSource*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlDataSourceSetActions(wl_client* client, wl_resource* resource, uint32_t dnd_actions) {
    const auto PO = (CWlDataSource*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setActions)
        PO->requests.setActions(PO, dnd_actions);
}

static void _CWlDataSource__DestroyListener(wl_listener* l, void* d) {
    CWlDataSourceDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlDataSource* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlDataSourceVTable[] = {
    (void*)_CWlDataSourceOffer,
    (void*)_CWlDataSourceDestroy,
    (void*)_CWlDataSourceSetActions,
};

void CWlDataSource::sendTarget(const char* mime_type) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mime_type);
}

void CWlDataSource::sendSend(const char* mime_type, int32_t fd) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, mime_type, fd);
}

void CWlDataSource::sendCancelled() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlDataSource::sendDndDropPerformed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CWlDataSource::sendDndFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CWlDataSource::sendAction(uint32_t dnd_action) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, dnd_action);
}

void CWlDataSource::sendTargetRaw(const char* mime_type) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mime_type);
}

void CWlDataSource::sendSendRaw(const char* mime_type, int32_t fd) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, mime_type, fd);
}

void CWlDataSource::sendCancelledRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlDataSource::sendDndDropPerformedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CWlDataSource::sendDndFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CWlDataSource::sendActionRaw(uint32_t dnd_action) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, dnd_action);
}
static const wl_interface* _CWlDataSourceOfferTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDataSourceSetActionsTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDataSourceTargetTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDataSourceSendTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDataSourceActionTypes[] = {
    nullptr,
};

static const wl_message _CWlDataSourceRequests[] = {
    { "offer", "s", _CWlDataSourceOfferTypes + 0},
    { "destroy", "", dummyTypes + 0},
    { "set_actions", "3u", _CWlDataSourceSetActionsTypes + 0},
};

static const wl_message _CWlDataSourceEvents[] = {
    { "target", "?s", _CWlDataSourceTargetTypes + 0},
    { "send", "sh", _CWlDataSourceSendTypes + 0},
    { "cancelled", "", dummyTypes + 0},
    { "dnd_drop_performed", "3", dummyTypes + 0},
    { "dnd_finished", "3", dummyTypes + 0},
    { "action", "3u", _CWlDataSourceActionTypes + 0},
};

const wl_interface wl_data_source_interface = {
    "wl_data_source", 3,
    3, _CWlDataSourceRequests,
    6, _CWlDataSourceEvents,
};

CWlDataSource::CWlDataSource(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_data_source_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlDataSource__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlDataSourceVTable, this, nullptr);
}

CWlDataSource::~CWlDataSource() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlDataSource::onDestroyCalled() {
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

void CWlDataSource::setOffer(F<void(CWlDataSource*, const char*)> handler) {
    requests.offer = handler;
}

void CWlDataSource::setDestroy(F<void(CWlDataSource*)> handler) {
    requests.destroy = handler;
}

void CWlDataSource::setSetActions(F<void(CWlDataSource*, uint32_t)> handler) {
    requests.setActions = handler;
}

static void _CWlDataDeviceStartDrag(wl_client* client, wl_resource* resource, wl_resource* source, wl_resource* origin, wl_resource* icon, uint32_t serial) {
    const auto PO = (CWlDataDevice*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.startDrag)
        PO->requests.startDrag(PO, source, origin, icon, serial);
}

static void _CWlDataDeviceSetSelection(wl_client* client, wl_resource* resource, wl_resource* source, uint32_t serial) {
    const auto PO = (CWlDataDevice*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSelection)
        PO->requests.setSelection(PO, source, serial);
}

static void _CWlDataDeviceRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlDataDevice*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlDataDevice__DestroyListener(wl_listener* l, void* d) {
    CWlDataDeviceDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlDataDevice* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlDataDeviceVTable[] = {
    (void*)_CWlDataDeviceStartDrag,
    (void*)_CWlDataDeviceSetSelection,
    (void*)_CWlDataDeviceRelease,
};

void CWlDataDevice::sendDataOffer(CWlDataOffer* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, id ? id->pResource : nullptr);
}

void CWlDataDevice::sendEnter(uint32_t serial, CWlSurface* surface, wl_fixed_t x, wl_fixed_t y, CWlDataOffer* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, surface ? surface->pResource : nullptr, x, y, id ? id->pResource : nullptr);
}

void CWlDataDevice::sendLeave() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlDataDevice::sendMotion(uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, time, x, y);
}

void CWlDataDevice::sendDrop() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CWlDataDevice::sendSelection(CWlDataOffer* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, id ? id->pResource : nullptr);
}

void CWlDataDevice::sendDataOfferRaw(CWlDataOffer* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, id);
}

void CWlDataDevice::sendEnterRaw(uint32_t serial, wl_resource* surface, wl_fixed_t x, wl_fixed_t y, wl_resource* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, surface, x, y, id);
}

void CWlDataDevice::sendLeaveRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlDataDevice::sendMotionRaw(uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, time, x, y);
}

void CWlDataDevice::sendDropRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CWlDataDevice::sendSelectionRaw(wl_resource* id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, id);
}
static const wl_interface* _CWlDataDeviceStartDragTypes[] = {
    &wl_data_source_interface,
    &wl_surface_interface,
    &wl_surface_interface,
    nullptr,
};
static const wl_interface* _CWlDataDeviceSetSelectionTypes[] = {
    &wl_data_source_interface,
    nullptr,
};
static const wl_interface* _CWlDataDeviceDataOfferTypes[] = {
    &wl_data_offer_interface,
};
static const wl_interface* _CWlDataDeviceEnterTypes[] = {
    nullptr,
    &wl_surface_interface,
    nullptr,
    nullptr,
    &wl_data_offer_interface,
};
static const wl_interface* _CWlDataDeviceMotionTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDataDeviceSelectionTypes[] = {
    &wl_data_offer_interface,
};

static const wl_message _CWlDataDeviceRequests[] = {
    { "start_drag", "?oo?ou", _CWlDataDeviceStartDragTypes + 0},
    { "set_selection", "?ou", _CWlDataDeviceSetSelectionTypes + 0},
    { "release", "2", dummyTypes + 0},
};

static const wl_message _CWlDataDeviceEvents[] = {
    { "data_offer", "n", _CWlDataDeviceDataOfferTypes + 0},
    { "enter", "uoff?o", _CWlDataDeviceEnterTypes + 0},
    { "leave", "", dummyTypes + 0},
    { "motion", "uff", _CWlDataDeviceMotionTypes + 0},
    { "drop", "", dummyTypes + 0},
    { "selection", "?o", _CWlDataDeviceSelectionTypes + 0},
};

const wl_interface wl_data_device_interface = {
    "wl_data_device", 3,
    3, _CWlDataDeviceRequests,
    6, _CWlDataDeviceEvents,
};

CWlDataDevice::CWlDataDevice(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_data_device_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlDataDevice__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlDataDeviceVTable, this, nullptr);
}

CWlDataDevice::~CWlDataDevice() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlDataDevice::onDestroyCalled() {
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

void CWlDataDevice::setStartDrag(F<void(CWlDataDevice*, wl_resource*, wl_resource*, wl_resource*, uint32_t)> handler) {
    requests.startDrag = handler;
}

void CWlDataDevice::setSetSelection(F<void(CWlDataDevice*, wl_resource*, uint32_t)> handler) {
    requests.setSelection = handler;
}

void CWlDataDevice::setRelease(F<void(CWlDataDevice*)> handler) {
    requests.release = handler;
}

static void _CWlDataDeviceManagerCreateDataSource(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlDataDeviceManager*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createDataSource)
        PO->requests.createDataSource(PO, id);
}

static void _CWlDataDeviceManagerGetDataDevice(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* seat) {
    const auto PO = (CWlDataDeviceManager*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getDataDevice)
        PO->requests.getDataDevice(PO, id, seat);
}

static void _CWlDataDeviceManager__DestroyListener(wl_listener* l, void* d) {
    CWlDataDeviceManagerDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlDataDeviceManager* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlDataDeviceManagerVTable[] = {
    (void*)_CWlDataDeviceManagerCreateDataSource,
    (void*)_CWlDataDeviceManagerGetDataDevice,
};
static const wl_interface* _CWlDataDeviceManagerCreateDataSourceTypes[] = {
    &wl_data_source_interface,
};
static const wl_interface* _CWlDataDeviceManagerGetDataDeviceTypes[] = {
    &wl_data_device_interface,
    &wl_seat_interface,
};

static const wl_message _CWlDataDeviceManagerRequests[] = {
    { "create_data_source", "n", _CWlDataDeviceManagerCreateDataSourceTypes + 0},
    { "get_data_device", "no", _CWlDataDeviceManagerGetDataDeviceTypes + 0},
};

const wl_interface wl_data_device_manager_interface = {
    "wl_data_device_manager", 3,
    2, _CWlDataDeviceManagerRequests,
    0, nullptr,
};

CWlDataDeviceManager::CWlDataDeviceManager(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_data_device_manager_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlDataDeviceManager__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlDataDeviceManagerVTable, this, nullptr);
}

CWlDataDeviceManager::~CWlDataDeviceManager() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlDataDeviceManager::onDestroyCalled() {
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

void CWlDataDeviceManager::setCreateDataSource(F<void(CWlDataDeviceManager*, uint32_t)> handler) {
    requests.createDataSource = handler;
}

void CWlDataDeviceManager::setGetDataDevice(F<void(CWlDataDeviceManager*, uint32_t, wl_resource*)> handler) {
    requests.getDataDevice = handler;
}

static void _CWlShellGetShellSurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CWlShell*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getShellSurface)
        PO->requests.getShellSurface(PO, id, surface);
}

static void _CWlShell__DestroyListener(wl_listener* l, void* d) {
    CWlShellDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlShell* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlShellVTable[] = {
    (void*)_CWlShellGetShellSurface,
};
static const wl_interface* _CWlShellGetShellSurfaceTypes[] = {
    &wl_shell_surface_interface,
    &wl_surface_interface,
};

static const wl_message _CWlShellRequests[] = {
    { "get_shell_surface", "no", _CWlShellGetShellSurfaceTypes + 0},
};

const wl_interface wl_shell_interface = {
    "wl_shell", 1,
    1, _CWlShellRequests,
    0, nullptr,
};

CWlShell::CWlShell(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_shell_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlShell__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlShellVTable, this, nullptr);
}

CWlShell::~CWlShell() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlShell::onDestroyCalled() {
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

void CWlShell::setGetShellSurface(F<void(CWlShell*, uint32_t, wl_resource*)> handler) {
    requests.getShellSurface = handler;
}

static void _CWlShellSurfacePong(wl_client* client, wl_resource* resource, uint32_t serial) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.pong)
        PO->requests.pong(PO, serial);
}

static void _CWlShellSurfaceMove(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.move)
        PO->requests.move(PO, seat, serial);
}

static void _CWlShellSurfaceResize(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial, enum wl_shell_surface_resize edges) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.resize)
        PO->requests.resize(PO, seat, serial, edges);
}

static void _CWlShellSurfaceSetToplevel(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setToplevel)
        PO->requests.setToplevel(PO);
}

static void _CWlShellSurfaceSetTransient(wl_client* client, wl_resource* resource, wl_resource* parent, int32_t x, int32_t y, enum wl_shell_surface_transient flags) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setTransient)
        PO->requests.setTransient(PO, parent, x, y, flags);
}

static void _CWlShellSurfaceSetFullscreen(wl_client* client, wl_resource* resource, enum wl_shell_surface_fullscreen_method method, uint32_t framerate, wl_resource* output) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setFullscreen)
        PO->requests.setFullscreen(PO, method, framerate, output);
}

static void _CWlShellSurfaceSetPopup(wl_client* client, wl_resource* resource, wl_resource* seat, uint32_t serial, wl_resource* parent, int32_t x, int32_t y, enum wl_shell_surface_transient flags) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setPopup)
        PO->requests.setPopup(PO, seat, serial, parent, x, y, flags);
}

static void _CWlShellSurfaceSetMaximized(wl_client* client, wl_resource* resource, wl_resource* output) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMaximized)
        PO->requests.setMaximized(PO, output);
}

static void _CWlShellSurfaceSetTitle(wl_client* client, wl_resource* resource, const char* title) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setTitle)
        PO->requests.setTitle(PO, title);
}

static void _CWlShellSurfaceSetClass(wl_client* client, wl_resource* resource, const char* class_) {
    const auto PO = (CWlShellSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setClass)
        PO->requests.setClass(PO, class_);
}

static void _CWlShellSurface__DestroyListener(wl_listener* l, void* d) {
    CWlShellSurfaceDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlShellSurface* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlShellSurfaceVTable[] = {
    (void*)_CWlShellSurfacePong,
    (void*)_CWlShellSurfaceMove,
    (void*)_CWlShellSurfaceResize,
    (void*)_CWlShellSurfaceSetToplevel,
    (void*)_CWlShellSurfaceSetTransient,
    (void*)_CWlShellSurfaceSetFullscreen,
    (void*)_CWlShellSurfaceSetPopup,
    (void*)_CWlShellSurfaceSetMaximized,
    (void*)_CWlShellSurfaceSetTitle,
    (void*)_CWlShellSurfaceSetClass,
};

void CWlShellSurface::sendPing(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial);
}

void CWlShellSurface::sendConfigure(enum wl_shell_surface_resize edges, int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, edges, width, height);
}

void CWlShellSurface::sendPopupDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlShellSurface::sendPingRaw(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial);
}

void CWlShellSurface::sendConfigureRaw(enum wl_shell_surface_resize edges, int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, edges, width, height);
}

void CWlShellSurface::sendPopupDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}
static const wl_interface* _CWlShellSurfacePongTypes[] = {
    nullptr,
};
static const wl_interface* _CWlShellSurfaceMoveTypes[] = {
    &wl_seat_interface,
    nullptr,
};
static const wl_interface* _CWlShellSurfaceResizeTypes[] = {
    &wl_seat_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlShellSurfaceSetTransientTypes[] = {
    &wl_surface_interface,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlShellSurfaceSetFullscreenTypes[] = {
    nullptr,
    nullptr,
    &wl_output_interface,
};
static const wl_interface* _CWlShellSurfaceSetPopupTypes[] = {
    &wl_seat_interface,
    nullptr,
    &wl_surface_interface,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlShellSurfaceSetMaximizedTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CWlShellSurfaceSetTitleTypes[] = {
    nullptr,
};
static const wl_interface* _CWlShellSurfaceSetClassTypes[] = {
    nullptr,
};
static const wl_interface* _CWlShellSurfacePingTypes[] = {
    nullptr,
};
static const wl_interface* _CWlShellSurfaceConfigureTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CWlShellSurfaceRequests[] = {
    { "pong", "u", _CWlShellSurfacePongTypes + 0},
    { "move", "ou", _CWlShellSurfaceMoveTypes + 0},
    { "resize", "ouu", _CWlShellSurfaceResizeTypes + 0},
    { "set_toplevel", "", dummyTypes + 0},
    { "set_transient", "oiiu", _CWlShellSurfaceSetTransientTypes + 0},
    { "set_fullscreen", "uu?o", _CWlShellSurfaceSetFullscreenTypes + 0},
    { "set_popup", "ouoiiu", _CWlShellSurfaceSetPopupTypes + 0},
    { "set_maximized", "?o", _CWlShellSurfaceSetMaximizedTypes + 0},
    { "set_title", "s", _CWlShellSurfaceSetTitleTypes + 0},
    { "set_class", "s", _CWlShellSurfaceSetClassTypes + 0},
};

static const wl_message _CWlShellSurfaceEvents[] = {
    { "ping", "u", _CWlShellSurfacePingTypes + 0},
    { "configure", "uii", _CWlShellSurfaceConfigureTypes + 0},
    { "popup_done", "", dummyTypes + 0},
};

const wl_interface wl_shell_surface_interface = {
    "wl_shell_surface", 1,
    10, _CWlShellSurfaceRequests,
    3, _CWlShellSurfaceEvents,
};

CWlShellSurface::CWlShellSurface(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_shell_surface_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlShellSurface__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlShellSurfaceVTable, this, nullptr);
}

CWlShellSurface::~CWlShellSurface() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlShellSurface::onDestroyCalled() {
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

void CWlShellSurface::setPong(F<void(CWlShellSurface*, uint32_t)> handler) {
    requests.pong = handler;
}

void CWlShellSurface::setMove(F<void(CWlShellSurface*, wl_resource*, uint32_t)> handler) {
    requests.move = handler;
}

void CWlShellSurface::setResize(F<void(CWlShellSurface*, wl_resource*, uint32_t, enum wl_shell_surface_resize)> handler) {
    requests.resize = handler;
}

void CWlShellSurface::setSetToplevel(F<void(CWlShellSurface*)> handler) {
    requests.setToplevel = handler;
}

void CWlShellSurface::setSetTransient(F<void(CWlShellSurface*, wl_resource*, int32_t, int32_t, enum wl_shell_surface_transient)> handler) {
    requests.setTransient = handler;
}

void CWlShellSurface::setSetFullscreen(F<void(CWlShellSurface*, enum wl_shell_surface_fullscreen_method, uint32_t, wl_resource*)> handler) {
    requests.setFullscreen = handler;
}

void CWlShellSurface::setSetPopup(F<void(CWlShellSurface*, wl_resource*, uint32_t, wl_resource*, int32_t, int32_t, enum wl_shell_surface_transient)> handler) {
    requests.setPopup = handler;
}

void CWlShellSurface::setSetMaximized(F<void(CWlShellSurface*, wl_resource*)> handler) {
    requests.setMaximized = handler;
}

void CWlShellSurface::setSetTitle(F<void(CWlShellSurface*, const char*)> handler) {
    requests.setTitle = handler;
}

void CWlShellSurface::setSetClass(F<void(CWlShellSurface*, const char*)> handler) {
    requests.setClass = handler;
}

static void _CWlSurfaceDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlSurfaceAttach(wl_client* client, wl_resource* resource, wl_resource* buffer, int32_t x, int32_t y) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.attach)
        PO->requests.attach(PO, buffer, x, y);
}

static void _CWlSurfaceDamage(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.damage)
        PO->requests.damage(PO, x, y, width, height);
}

static void _CWlSurfaceFrame(wl_client* client, wl_resource* resource, uint32_t callback) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.frame)
        PO->requests.frame(PO, callback);
}

static void _CWlSurfaceSetOpaqueRegion(wl_client* client, wl_resource* resource, wl_resource* region) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setOpaqueRegion)
        PO->requests.setOpaqueRegion(PO, region);
}

static void _CWlSurfaceSetInputRegion(wl_client* client, wl_resource* resource, wl_resource* region) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setInputRegion)
        PO->requests.setInputRegion(PO, region);
}

static void _CWlSurfaceCommit(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.commit)
        PO->requests.commit(PO);
}

static void _CWlSurfaceSetBufferTransform(wl_client* client, wl_resource* resource, int32_t transform) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setBufferTransform)
        PO->requests.setBufferTransform(PO, transform);
}

static void _CWlSurfaceSetBufferScale(wl_client* client, wl_resource* resource, int32_t scale) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setBufferScale)
        PO->requests.setBufferScale(PO, scale);
}

static void _CWlSurfaceDamageBuffer(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.damageBuffer)
        PO->requests.damageBuffer(PO, x, y, width, height);
}

static void _CWlSurfaceOffset(wl_client* client, wl_resource* resource, int32_t x, int32_t y) {
    const auto PO = (CWlSurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.offset)
        PO->requests.offset(PO, x, y);
}

static void _CWlSurface__DestroyListener(wl_listener* l, void* d) {
    CWlSurfaceDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlSurface* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlSurfaceVTable[] = {
    (void*)_CWlSurfaceDestroy,
    (void*)_CWlSurfaceAttach,
    (void*)_CWlSurfaceDamage,
    (void*)_CWlSurfaceFrame,
    (void*)_CWlSurfaceSetOpaqueRegion,
    (void*)_CWlSurfaceSetInputRegion,
    (void*)_CWlSurfaceCommit,
    (void*)_CWlSurfaceSetBufferTransform,
    (void*)_CWlSurfaceSetBufferScale,
    (void*)_CWlSurfaceDamageBuffer,
    (void*)_CWlSurfaceOffset,
};

void CWlSurface::sendEnter(CWlOutput* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, output ? output->pResource : nullptr);
}

void CWlSurface::sendLeave(CWlOutput* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, output ? output->pResource : nullptr);
}

void CWlSurface::sendPreferredBufferScale(int32_t factor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, factor);
}

void CWlSurface::sendPreferredBufferTransform(uint32_t transform) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, transform);
}

void CWlSurface::sendEnterRaw(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, output);
}

void CWlSurface::sendLeaveRaw(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, output);
}

void CWlSurface::sendPreferredBufferScaleRaw(int32_t factor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, factor);
}

void CWlSurface::sendPreferredBufferTransformRaw(uint32_t transform) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, transform);
}
static const wl_interface* _CWlSurfaceAttachTypes[] = {
    &wl_buffer_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlSurfaceDamageTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlSurfaceFrameTypes[] = {
    &wl_callback_interface,
};
static const wl_interface* _CWlSurfaceSetOpaqueRegionTypes[] = {
    &wl_region_interface,
};
static const wl_interface* _CWlSurfaceSetInputRegionTypes[] = {
    &wl_region_interface,
};
static const wl_interface* _CWlSurfaceSetBufferTransformTypes[] = {
    nullptr,
};
static const wl_interface* _CWlSurfaceSetBufferScaleTypes[] = {
    nullptr,
};
static const wl_interface* _CWlSurfaceDamageBufferTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlSurfaceOffsetTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlSurfaceEnterTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CWlSurfaceLeaveTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CWlSurfacePreferredBufferScaleTypes[] = {
    nullptr,
};
static const wl_interface* _CWlSurfacePreferredBufferTransformTypes[] = {
    nullptr,
};

static const wl_message _CWlSurfaceRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "attach", "?oii", _CWlSurfaceAttachTypes + 0},
    { "damage", "iiii", _CWlSurfaceDamageTypes + 0},
    { "frame", "n", _CWlSurfaceFrameTypes + 0},
    { "set_opaque_region", "?o", _CWlSurfaceSetOpaqueRegionTypes + 0},
    { "set_input_region", "?o", _CWlSurfaceSetInputRegionTypes + 0},
    { "commit", "", dummyTypes + 0},
    { "set_buffer_transform", "2i", _CWlSurfaceSetBufferTransformTypes + 0},
    { "set_buffer_scale", "3i", _CWlSurfaceSetBufferScaleTypes + 0},
    { "damage_buffer", "4iiii", _CWlSurfaceDamageBufferTypes + 0},
    { "offset", "5ii", _CWlSurfaceOffsetTypes + 0},
};

static const wl_message _CWlSurfaceEvents[] = {
    { "enter", "o", _CWlSurfaceEnterTypes + 0},
    { "leave", "o", _CWlSurfaceLeaveTypes + 0},
    { "preferred_buffer_scale", "6i", _CWlSurfacePreferredBufferScaleTypes + 0},
    { "preferred_buffer_transform", "6u", _CWlSurfacePreferredBufferTransformTypes + 0},
};

const wl_interface wl_surface_interface = {
    "wl_surface", 6,
    11, _CWlSurfaceRequests,
    4, _CWlSurfaceEvents,
};

CWlSurface::CWlSurface(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_surface_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlSurface__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlSurfaceVTable, this, nullptr);
}

CWlSurface::~CWlSurface() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlSurface::onDestroyCalled() {
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

void CWlSurface::setDestroy(F<void(CWlSurface*)> handler) {
    requests.destroy = handler;
}

void CWlSurface::setAttach(F<void(CWlSurface*, wl_resource*, int32_t, int32_t)> handler) {
    requests.attach = handler;
}

void CWlSurface::setDamage(F<void(CWlSurface*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.damage = handler;
}

void CWlSurface::setFrame(F<void(CWlSurface*, uint32_t)> handler) {
    requests.frame = handler;
}

void CWlSurface::setSetOpaqueRegion(F<void(CWlSurface*, wl_resource*)> handler) {
    requests.setOpaqueRegion = handler;
}

void CWlSurface::setSetInputRegion(F<void(CWlSurface*, wl_resource*)> handler) {
    requests.setInputRegion = handler;
}

void CWlSurface::setCommit(F<void(CWlSurface*)> handler) {
    requests.commit = handler;
}

void CWlSurface::setSetBufferTransform(F<void(CWlSurface*, int32_t)> handler) {
    requests.setBufferTransform = handler;
}

void CWlSurface::setSetBufferScale(F<void(CWlSurface*, int32_t)> handler) {
    requests.setBufferScale = handler;
}

void CWlSurface::setDamageBuffer(F<void(CWlSurface*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.damageBuffer = handler;
}

void CWlSurface::setOffset(F<void(CWlSurface*, int32_t, int32_t)> handler) {
    requests.offset = handler;
}

static void _CWlSeatGetPointer(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlSeat*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getPointer)
        PO->requests.getPointer(PO, id);
}

static void _CWlSeatGetKeyboard(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlSeat*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getKeyboard)
        PO->requests.getKeyboard(PO, id);
}

static void _CWlSeatGetTouch(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlSeat*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getTouch)
        PO->requests.getTouch(PO, id);
}

static void _CWlSeatRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSeat*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlSeat__DestroyListener(wl_listener* l, void* d) {
    CWlSeatDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlSeat* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlSeatVTable[] = {
    (void*)_CWlSeatGetPointer,
    (void*)_CWlSeatGetKeyboard,
    (void*)_CWlSeatGetTouch,
    (void*)_CWlSeatRelease,
};

void CWlSeat::sendCapabilities(enum wl_seat_capability capabilities) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, capabilities);
}

void CWlSeat::sendName(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, name);
}

void CWlSeat::sendCapabilitiesRaw(enum wl_seat_capability capabilities) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, capabilities);
}

void CWlSeat::sendNameRaw(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, name);
}
static const wl_interface* _CWlSeatGetPointerTypes[] = {
    &wl_pointer_interface,
};
static const wl_interface* _CWlSeatGetKeyboardTypes[] = {
    &wl_keyboard_interface,
};
static const wl_interface* _CWlSeatGetTouchTypes[] = {
    &wl_touch_interface,
};
static const wl_interface* _CWlSeatCapabilitiesTypes[] = {
    nullptr,
};
static const wl_interface* _CWlSeatNameTypes[] = {
    nullptr,
};

static const wl_message _CWlSeatRequests[] = {
    { "get_pointer", "n", _CWlSeatGetPointerTypes + 0},
    { "get_keyboard", "n", _CWlSeatGetKeyboardTypes + 0},
    { "get_touch", "n", _CWlSeatGetTouchTypes + 0},
    { "release", "5", dummyTypes + 0},
};

static const wl_message _CWlSeatEvents[] = {
    { "capabilities", "u", _CWlSeatCapabilitiesTypes + 0},
    { "name", "2s", _CWlSeatNameTypes + 0},
};

const wl_interface wl_seat_interface = {
    "wl_seat", 9,
    4, _CWlSeatRequests,
    2, _CWlSeatEvents,
};

CWlSeat::CWlSeat(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_seat_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlSeat__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlSeatVTable, this, nullptr);
}

CWlSeat::~CWlSeat() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlSeat::onDestroyCalled() {
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

void CWlSeat::setGetPointer(F<void(CWlSeat*, uint32_t)> handler) {
    requests.getPointer = handler;
}

void CWlSeat::setGetKeyboard(F<void(CWlSeat*, uint32_t)> handler) {
    requests.getKeyboard = handler;
}

void CWlSeat::setGetTouch(F<void(CWlSeat*, uint32_t)> handler) {
    requests.getTouch = handler;
}

void CWlSeat::setRelease(F<void(CWlSeat*)> handler) {
    requests.release = handler;
}

static void _CWlPointerSetCursor(wl_client* client, wl_resource* resource, uint32_t serial, wl_resource* surface, int32_t hotspot_x, int32_t hotspot_y) {
    const auto PO = (CWlPointer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setCursor)
        PO->requests.setCursor(PO, serial, surface, hotspot_x, hotspot_y);
}

static void _CWlPointerRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlPointer*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlPointer__DestroyListener(wl_listener* l, void* d) {
    CWlPointerDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlPointer* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlPointerVTable[] = {
    (void*)_CWlPointerSetCursor,
    (void*)_CWlPointerRelease,
};

void CWlPointer::sendEnter(uint32_t serial, CWlSurface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, surface ? surface->pResource : nullptr, surface_x, surface_y);
}

void CWlPointer::sendLeave(uint32_t serial, CWlSurface* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, surface ? surface->pResource : nullptr);
}

void CWlPointer::sendMotion(uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, time, surface_x, surface_y);
}

void CWlPointer::sendButton(uint32_t serial, uint32_t time, uint32_t button, enum wl_pointer_button_state state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, serial, time, button, state);
}

void CWlPointer::sendAxis(uint32_t time, enum wl_pointer_axis axis, wl_fixed_t value) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, time, axis, value);
}

void CWlPointer::sendFrame() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CWlPointer::sendAxisSource(enum wl_pointer_axis_source axis_source) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, axis_source);
}

void CWlPointer::sendAxisStop(uint32_t time, enum wl_pointer_axis axis) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7, time, axis);
}

void CWlPointer::sendAxisDiscrete(enum wl_pointer_axis axis, int32_t discrete) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 8, axis, discrete);
}

void CWlPointer::sendAxisValue120(enum wl_pointer_axis axis, int32_t value120) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 9, axis, value120);
}

void CWlPointer::sendAxisRelativeDirection(enum wl_pointer_axis axis, enum wl_pointer_axis_relative_direction direction) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 10, axis, direction);
}

void CWlPointer::sendEnterRaw(uint32_t serial, wl_resource* surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, surface, surface_x, surface_y);
}

void CWlPointer::sendLeaveRaw(uint32_t serial, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, surface);
}

void CWlPointer::sendMotionRaw(uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, time, surface_x, surface_y);
}

void CWlPointer::sendButtonRaw(uint32_t serial, uint32_t time, uint32_t button, enum wl_pointer_button_state state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, serial, time, button, state);
}

void CWlPointer::sendAxisRaw(uint32_t time, enum wl_pointer_axis axis, wl_fixed_t value) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, time, axis, value);
}

void CWlPointer::sendFrameRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5);
}

void CWlPointer::sendAxisSourceRaw(enum wl_pointer_axis_source axis_source) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, axis_source);
}

void CWlPointer::sendAxisStopRaw(uint32_t time, enum wl_pointer_axis axis) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7, time, axis);
}

void CWlPointer::sendAxisDiscreteRaw(enum wl_pointer_axis axis, int32_t discrete) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 8, axis, discrete);
}

void CWlPointer::sendAxisValue120Raw(enum wl_pointer_axis axis, int32_t value120) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 9, axis, value120);
}

void CWlPointer::sendAxisRelativeDirectionRaw(enum wl_pointer_axis axis, enum wl_pointer_axis_relative_direction direction) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 10, axis, direction);
}
static const wl_interface* _CWlPointerSetCursorTypes[] = {
    nullptr,
    &wl_surface_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerEnterTypes[] = {
    nullptr,
    &wl_surface_interface,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerLeaveTypes[] = {
    nullptr,
    &wl_surface_interface,
};
static const wl_interface* _CWlPointerMotionTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerButtonTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerAxisTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerAxisSourceTypes[] = {
    nullptr,
};
static const wl_interface* _CWlPointerAxisStopTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerAxisDiscreteTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerAxisValue120Types[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlPointerAxisRelativeDirectionTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CWlPointerRequests[] = {
    { "set_cursor", "u?oii", _CWlPointerSetCursorTypes + 0},
    { "release", "3", dummyTypes + 0},
};

static const wl_message _CWlPointerEvents[] = {
    { "enter", "uoff", _CWlPointerEnterTypes + 0},
    { "leave", "uo", _CWlPointerLeaveTypes + 0},
    { "motion", "uff", _CWlPointerMotionTypes + 0},
    { "button", "uuuu", _CWlPointerButtonTypes + 0},
    { "axis", "uuf", _CWlPointerAxisTypes + 0},
    { "frame", "5", dummyTypes + 0},
    { "axis_source", "5u", _CWlPointerAxisSourceTypes + 0},
    { "axis_stop", "5uu", _CWlPointerAxisStopTypes + 0},
    { "axis_discrete", "5ui", _CWlPointerAxisDiscreteTypes + 0},
    { "axis_value120", "8ui", _CWlPointerAxisValue120Types + 0},
    { "axis_relative_direction", "9uu", _CWlPointerAxisRelativeDirectionTypes + 0},
};

const wl_interface wl_pointer_interface = {
    "wl_pointer", 9,
    2, _CWlPointerRequests,
    11, _CWlPointerEvents,
};

CWlPointer::CWlPointer(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_pointer_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlPointer__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlPointerVTable, this, nullptr);
}

CWlPointer::~CWlPointer() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlPointer::onDestroyCalled() {
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

void CWlPointer::setSetCursor(F<void(CWlPointer*, uint32_t, wl_resource*, int32_t, int32_t)> handler) {
    requests.setCursor = handler;
}

void CWlPointer::setRelease(F<void(CWlPointer*)> handler) {
    requests.release = handler;
}

static void _CWlKeyboardRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlKeyboard*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlKeyboard__DestroyListener(wl_listener* l, void* d) {
    CWlKeyboardDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlKeyboard* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlKeyboardVTable[] = {
    (void*)_CWlKeyboardRelease,
};

void CWlKeyboard::sendKeymap(enum wl_keyboard_keymap_format format, int32_t fd, uint32_t size) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, format, fd, size);
}

void CWlKeyboard::sendEnter(uint32_t serial, CWlSurface* surface, wl_array* keys) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, surface ? surface->pResource : nullptr, keys);
}

void CWlKeyboard::sendLeave(uint32_t serial, CWlSurface* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, surface ? surface->pResource : nullptr);
}

void CWlKeyboard::sendKey(uint32_t serial, uint32_t time, uint32_t key, enum wl_keyboard_key_state state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, serial, time, key, state);
}

void CWlKeyboard::sendModifiers(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, serial, mods_depressed, mods_latched, mods_locked, group);
}

void CWlKeyboard::sendRepeatInfo(int32_t rate, int32_t delay) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, rate, delay);
}

void CWlKeyboard::sendKeymapRaw(enum wl_keyboard_keymap_format format, int32_t fd, uint32_t size) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, format, fd, size);
}

void CWlKeyboard::sendEnterRaw(uint32_t serial, wl_resource* surface, wl_array* keys) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, surface, keys);
}

void CWlKeyboard::sendLeaveRaw(uint32_t serial, wl_resource* surface) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, serial, surface);
}

void CWlKeyboard::sendKeyRaw(uint32_t serial, uint32_t time, uint32_t key, enum wl_keyboard_key_state state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, serial, time, key, state);
}

void CWlKeyboard::sendModifiersRaw(uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, serial, mods_depressed, mods_latched, mods_locked, group);
}

void CWlKeyboard::sendRepeatInfoRaw(int32_t rate, int32_t delay) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, rate, delay);
}
static const wl_interface* _CWlKeyboardKeymapTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlKeyboardEnterTypes[] = {
    nullptr,
    &wl_surface_interface,
    nullptr,
};
static const wl_interface* _CWlKeyboardLeaveTypes[] = {
    nullptr,
    &wl_surface_interface,
};
static const wl_interface* _CWlKeyboardKeyTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlKeyboardModifiersTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlKeyboardRepeatInfoTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CWlKeyboardRequests[] = {
    { "release", "3", dummyTypes + 0},
};

static const wl_message _CWlKeyboardEvents[] = {
    { "keymap", "uhu", _CWlKeyboardKeymapTypes + 0},
    { "enter", "uoa", _CWlKeyboardEnterTypes + 0},
    { "leave", "uo", _CWlKeyboardLeaveTypes + 0},
    { "key", "uuuu", _CWlKeyboardKeyTypes + 0},
    { "modifiers", "uuuuu", _CWlKeyboardModifiersTypes + 0},
    { "repeat_info", "4ii", _CWlKeyboardRepeatInfoTypes + 0},
};

const wl_interface wl_keyboard_interface = {
    "wl_keyboard", 9,
    1, _CWlKeyboardRequests,
    6, _CWlKeyboardEvents,
};

CWlKeyboard::CWlKeyboard(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_keyboard_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlKeyboard__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlKeyboardVTable, this, nullptr);
}

CWlKeyboard::~CWlKeyboard() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlKeyboard::onDestroyCalled() {
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

void CWlKeyboard::setRelease(F<void(CWlKeyboard*)> handler) {
    requests.release = handler;
}

static void _CWlTouchRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlTouch*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlTouch__DestroyListener(wl_listener* l, void* d) {
    CWlTouchDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlTouch* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlTouchVTable[] = {
    (void*)_CWlTouchRelease,
};

void CWlTouch::sendDown(uint32_t serial, uint32_t time, CWlSurface* surface, int32_t id, wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface ? surface->pResource : nullptr, id, x, y);
}

void CWlTouch::sendUp(uint32_t serial, uint32_t time, int32_t id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, time, id);
}

void CWlTouch::sendMotion(uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, time, id, x, y);
}

void CWlTouch::sendFrame() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CWlTouch::sendCancel() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CWlTouch::sendShape(int32_t id, wl_fixed_t major, wl_fixed_t minor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, id, major, minor);
}

void CWlTouch::sendOrientation(int32_t id, wl_fixed_t orientation) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, id, orientation);
}

void CWlTouch::sendDownRaw(uint32_t serial, uint32_t time, wl_resource* surface, int32_t id, wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, serial, time, surface, id, x, y);
}

void CWlTouch::sendUpRaw(uint32_t serial, uint32_t time, int32_t id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial, time, id);
}

void CWlTouch::sendMotionRaw(uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, time, id, x, y);
}

void CWlTouch::sendFrameRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CWlTouch::sendCancelRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4);
}

void CWlTouch::sendShapeRaw(int32_t id, wl_fixed_t major, wl_fixed_t minor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, id, major, minor);
}

void CWlTouch::sendOrientationRaw(int32_t id, wl_fixed_t orientation) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, id, orientation);
}
static const wl_interface* _CWlTouchDownTypes[] = {
    nullptr,
    nullptr,
    &wl_surface_interface,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlTouchUpTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlTouchMotionTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlTouchShapeTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlTouchOrientationTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CWlTouchRequests[] = {
    { "release", "3", dummyTypes + 0},
};

static const wl_message _CWlTouchEvents[] = {
    { "down", "uuoiff", _CWlTouchDownTypes + 0},
    { "up", "uui", _CWlTouchUpTypes + 0},
    { "motion", "uiff", _CWlTouchMotionTypes + 0},
    { "frame", "", dummyTypes + 0},
    { "cancel", "", dummyTypes + 0},
    { "shape", "6iff", _CWlTouchShapeTypes + 0},
    { "orientation", "6if", _CWlTouchOrientationTypes + 0},
};

const wl_interface wl_touch_interface = {
    "wl_touch", 9,
    1, _CWlTouchRequests,
    7, _CWlTouchEvents,
};

CWlTouch::CWlTouch(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_touch_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlTouch__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlTouchVTable, this, nullptr);
}

CWlTouch::~CWlTouch() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlTouch::onDestroyCalled() {
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

void CWlTouch::setRelease(F<void(CWlTouch*)> handler) {
    requests.release = handler;
}

static void _CWlOutputRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlOutput*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CWlOutput__DestroyListener(wl_listener* l, void* d) {
    CWlOutputDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlOutput* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlOutputVTable[] = {
    (void*)_CWlOutputRelease,
};

void CWlOutput::sendGeometry(int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char* make, const char* model, int32_t transform) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y, physical_width, physical_height, subpixel, make, model, transform);
}

void CWlOutput::sendMode(enum wl_output_mode flags, int32_t width, int32_t height, int32_t refresh) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, flags, width, height, refresh);
}

void CWlOutput::sendDone() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlOutput::sendScale(int32_t factor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, factor);
}

void CWlOutput::sendName(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, name);
}

void CWlOutput::sendDescription(const char* description) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, description);
}

void CWlOutput::sendGeometryRaw(int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char* make, const char* model, int32_t transform) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, x, y, physical_width, physical_height, subpixel, make, model, transform);
}

void CWlOutput::sendModeRaw(enum wl_output_mode flags, int32_t width, int32_t height, int32_t refresh) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, flags, width, height, refresh);
}

void CWlOutput::sendDoneRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlOutput::sendScaleRaw(int32_t factor) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, factor);
}

void CWlOutput::sendNameRaw(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, name);
}

void CWlOutput::sendDescriptionRaw(const char* description) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, description);
}
static const wl_interface* _CWlOutputGeometryTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlOutputModeTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlOutputScaleTypes[] = {
    nullptr,
};
static const wl_interface* _CWlOutputNameTypes[] = {
    nullptr,
};
static const wl_interface* _CWlOutputDescriptionTypes[] = {
    nullptr,
};

static const wl_message _CWlOutputRequests[] = {
    { "release", "3", dummyTypes + 0},
};

static const wl_message _CWlOutputEvents[] = {
    { "geometry", "iiiiissi", _CWlOutputGeometryTypes + 0},
    { "mode", "uiii", _CWlOutputModeTypes + 0},
    { "done", "2", dummyTypes + 0},
    { "scale", "2i", _CWlOutputScaleTypes + 0},
    { "name", "4s", _CWlOutputNameTypes + 0},
    { "description", "4s", _CWlOutputDescriptionTypes + 0},
};

const wl_interface wl_output_interface = {
    "wl_output", 4,
    1, _CWlOutputRequests,
    6, _CWlOutputEvents,
};

CWlOutput::CWlOutput(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_output_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlOutput__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlOutputVTable, this, nullptr);
}

CWlOutput::~CWlOutput() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlOutput::onDestroyCalled() {
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

void CWlOutput::setRelease(F<void(CWlOutput*)> handler) {
    requests.release = handler;
}

static void _CWlRegionDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlRegion*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlRegionAdd(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CWlRegion*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.add)
        PO->requests.add(PO, x, y, width, height);
}

static void _CWlRegionSubtract(wl_client* client, wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    const auto PO = (CWlRegion*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.subtract)
        PO->requests.subtract(PO, x, y, width, height);
}

static void _CWlRegion__DestroyListener(wl_listener* l, void* d) {
    CWlRegionDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlRegion* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlRegionVTable[] = {
    (void*)_CWlRegionDestroy,
    (void*)_CWlRegionAdd,
    (void*)_CWlRegionSubtract,
};
static const wl_interface* _CWlRegionAddTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlRegionSubtractTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CWlRegionRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "add", "iiii", _CWlRegionAddTypes + 0},
    { "subtract", "iiii", _CWlRegionSubtractTypes + 0},
};

const wl_interface wl_region_interface = {
    "wl_region", 1,
    3, _CWlRegionRequests,
    0, nullptr,
};

CWlRegion::CWlRegion(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_region_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlRegion__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlRegionVTable, this, nullptr);
}

CWlRegion::~CWlRegion() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlRegion::onDestroyCalled() {
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

void CWlRegion::setDestroy(F<void(CWlRegion*)> handler) {
    requests.destroy = handler;
}

void CWlRegion::setAdd(F<void(CWlRegion*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.add = handler;
}

void CWlRegion::setSubtract(F<void(CWlRegion*, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.subtract = handler;
}

static void _CWlSubcompositorDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSubcompositor*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlSubcompositorGetSubsurface(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface, wl_resource* parent) {
    const auto PO = (CWlSubcompositor*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getSubsurface)
        PO->requests.getSubsurface(PO, id, surface, parent);
}

static void _CWlSubcompositor__DestroyListener(wl_listener* l, void* d) {
    CWlSubcompositorDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlSubcompositor* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlSubcompositorVTable[] = {
    (void*)_CWlSubcompositorDestroy,
    (void*)_CWlSubcompositorGetSubsurface,
};
static const wl_interface* _CWlSubcompositorGetSubsurfaceTypes[] = {
    &wl_subsurface_interface,
    &wl_surface_interface,
    &wl_surface_interface,
};

static const wl_message _CWlSubcompositorRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_subsurface", "noo", _CWlSubcompositorGetSubsurfaceTypes + 0},
};

const wl_interface wl_subcompositor_interface = {
    "wl_subcompositor", 1,
    2, _CWlSubcompositorRequests,
    0, nullptr,
};

CWlSubcompositor::CWlSubcompositor(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_subcompositor_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlSubcompositor__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlSubcompositorVTable, this, nullptr);
}

CWlSubcompositor::~CWlSubcompositor() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlSubcompositor::onDestroyCalled() {
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

void CWlSubcompositor::setDestroy(F<void(CWlSubcompositor*)> handler) {
    requests.destroy = handler;
}

void CWlSubcompositor::setGetSubsurface(F<void(CWlSubcompositor*, uint32_t, wl_resource*, wl_resource*)> handler) {
    requests.getSubsurface = handler;
}

static void _CWlSubsurfaceDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSubsurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWlSubsurfaceSetPosition(wl_client* client, wl_resource* resource, int32_t x, int32_t y) {
    const auto PO = (CWlSubsurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setPosition)
        PO->requests.setPosition(PO, x, y);
}

static void _CWlSubsurfacePlaceAbove(wl_client* client, wl_resource* resource, wl_resource* sibling) {
    const auto PO = (CWlSubsurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.placeAbove)
        PO->requests.placeAbove(PO, sibling);
}

static void _CWlSubsurfacePlaceBelow(wl_client* client, wl_resource* resource, wl_resource* sibling) {
    const auto PO = (CWlSubsurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.placeBelow)
        PO->requests.placeBelow(PO, sibling);
}

static void _CWlSubsurfaceSetSync(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSubsurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSync)
        PO->requests.setSync(PO);
}

static void _CWlSubsurfaceSetDesync(wl_client* client, wl_resource* resource) {
    const auto PO = (CWlSubsurface*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setDesync)
        PO->requests.setDesync(PO);
}

static void _CWlSubsurface__DestroyListener(wl_listener* l, void* d) {
    CWlSubsurfaceDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlSubsurface* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlSubsurfaceVTable[] = {
    (void*)_CWlSubsurfaceDestroy,
    (void*)_CWlSubsurfaceSetPosition,
    (void*)_CWlSubsurfacePlaceAbove,
    (void*)_CWlSubsurfacePlaceBelow,
    (void*)_CWlSubsurfaceSetSync,
    (void*)_CWlSubsurfaceSetDesync,
};
static const wl_interface* _CWlSubsurfaceSetPositionTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CWlSubsurfacePlaceAboveTypes[] = {
    &wl_surface_interface,
};
static const wl_interface* _CWlSubsurfacePlaceBelowTypes[] = {
    &wl_surface_interface,
};

static const wl_message _CWlSubsurfaceRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_position", "ii", _CWlSubsurfaceSetPositionTypes + 0},
    { "place_above", "o", _CWlSubsurfacePlaceAboveTypes + 0},
    { "place_below", "o", _CWlSubsurfacePlaceBelowTypes + 0},
    { "set_sync", "", dummyTypes + 0},
    { "set_desync", "", dummyTypes + 0},
};

const wl_interface wl_subsurface_interface = {
    "wl_subsurface", 1,
    6, _CWlSubsurfaceRequests,
    0, nullptr,
};

CWlSubsurface::CWlSubsurface(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_subsurface_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlSubsurface__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlSubsurfaceVTable, this, nullptr);
}

CWlSubsurface::~CWlSubsurface() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlSubsurface::onDestroyCalled() {
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

void CWlSubsurface::setDestroy(F<void(CWlSubsurface*)> handler) {
    requests.destroy = handler;
}

void CWlSubsurface::setSetPosition(F<void(CWlSubsurface*, int32_t, int32_t)> handler) {
    requests.setPosition = handler;
}

void CWlSubsurface::setPlaceAbove(F<void(CWlSubsurface*, wl_resource*)> handler) {
    requests.placeAbove = handler;
}

void CWlSubsurface::setPlaceBelow(F<void(CWlSubsurface*, wl_resource*)> handler) {
    requests.placeBelow = handler;
}

void CWlSubsurface::setSetSync(F<void(CWlSubsurface*)> handler) {
    requests.setSync = handler;
}

void CWlSubsurface::setSetDesync(F<void(CWlSubsurface*)> handler) {
    requests.setDesync = handler;
}

#undef F
