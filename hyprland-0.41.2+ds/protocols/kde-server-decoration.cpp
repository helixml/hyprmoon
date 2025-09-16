// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// server_decoration

/*
 This protocol's authors' copyright notice is:


    SPDX-FileCopyrightText: 2015 Martin Gräßlin

    SPDX-License-Identifier: LGPL-2.1-or-later
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "kde-server-decoration.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface org_kde_kwin_server_decoration_manager_interface;
extern const wl_interface org_kde_kwin_server_decoration_interface;
extern const wl_interface wl_surface_interface;

static void _COrgKdeKwinServerDecorationManagerCreate(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (COrgKdeKwinServerDecorationManager*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.create)
        PO->requests.create(PO, id, surface);
}

static void _COrgKdeKwinServerDecorationManager__DestroyListener(wl_listener* l, void* d) {
    COrgKdeKwinServerDecorationManagerDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    COrgKdeKwinServerDecorationManager* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _COrgKdeKwinServerDecorationManagerVTable[] = {
    (void*)_COrgKdeKwinServerDecorationManagerCreate,
};

void COrgKdeKwinServerDecorationManager::sendDefaultMode(uint32_t mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}

void COrgKdeKwinServerDecorationManager::sendDefaultModeRaw(uint32_t mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}
static const wl_interface* _COrgKdeKwinServerDecorationManagerCreateTypes[] = {
    &org_kde_kwin_server_decoration_interface,
    &wl_surface_interface,
};
static const wl_interface* _COrgKdeKwinServerDecorationManagerDefaultModeTypes[] = {
    nullptr,
};

static const wl_message _COrgKdeKwinServerDecorationManagerRequests[] = {
    { "create", "no", _COrgKdeKwinServerDecorationManagerCreateTypes + 0},
};

static const wl_message _COrgKdeKwinServerDecorationManagerEvents[] = {
    { "default_mode", "u", _COrgKdeKwinServerDecorationManagerDefaultModeTypes + 0},
};

const wl_interface org_kde_kwin_server_decoration_manager_interface = {
    "org_kde_kwin_server_decoration_manager", 1,
    1, _COrgKdeKwinServerDecorationManagerRequests,
    1, _COrgKdeKwinServerDecorationManagerEvents,
};

COrgKdeKwinServerDecorationManager::COrgKdeKwinServerDecorationManager(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &org_kde_kwin_server_decoration_manager_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _COrgKdeKwinServerDecorationManager__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _COrgKdeKwinServerDecorationManagerVTable, this, nullptr);
}

COrgKdeKwinServerDecorationManager::~COrgKdeKwinServerDecorationManager() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void COrgKdeKwinServerDecorationManager::onDestroyCalled() {
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

void COrgKdeKwinServerDecorationManager::setCreate(F<void(COrgKdeKwinServerDecorationManager*, uint32_t, wl_resource*)> handler) {
    requests.create = handler;
}

static void _COrgKdeKwinServerDecorationRelease(wl_client* client, wl_resource* resource) {
    const auto PO = (COrgKdeKwinServerDecoration*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _COrgKdeKwinServerDecorationRequestMode(wl_client* client, wl_resource* resource, uint32_t mode) {
    const auto PO = (COrgKdeKwinServerDecoration*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.requestMode)
        PO->requests.requestMode(PO, mode);
}

static void _COrgKdeKwinServerDecoration__DestroyListener(wl_listener* l, void* d) {
    COrgKdeKwinServerDecorationDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    COrgKdeKwinServerDecoration* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _COrgKdeKwinServerDecorationVTable[] = {
    (void*)_COrgKdeKwinServerDecorationRelease,
    (void*)_COrgKdeKwinServerDecorationRequestMode,
};

void COrgKdeKwinServerDecoration::sendMode(uint32_t mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}

void COrgKdeKwinServerDecoration::sendModeRaw(uint32_t mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, mode);
}
static const wl_interface* _COrgKdeKwinServerDecorationRequestModeTypes[] = {
    nullptr,
};
static const wl_interface* _COrgKdeKwinServerDecorationModeTypes[] = {
    nullptr,
};

static const wl_message _COrgKdeKwinServerDecorationRequests[] = {
    { "release", "", dummyTypes + 0},
    { "request_mode", "u", _COrgKdeKwinServerDecorationRequestModeTypes + 0},
};

static const wl_message _COrgKdeKwinServerDecorationEvents[] = {
    { "mode", "u", _COrgKdeKwinServerDecorationModeTypes + 0},
};

const wl_interface org_kde_kwin_server_decoration_interface = {
    "org_kde_kwin_server_decoration", 1,
    2, _COrgKdeKwinServerDecorationRequests,
    1, _COrgKdeKwinServerDecorationEvents,
};

COrgKdeKwinServerDecoration::COrgKdeKwinServerDecoration(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &org_kde_kwin_server_decoration_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _COrgKdeKwinServerDecoration__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _COrgKdeKwinServerDecorationVTable, this, nullptr);
}

COrgKdeKwinServerDecoration::~COrgKdeKwinServerDecoration() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void COrgKdeKwinServerDecoration::onDestroyCalled() {
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

void COrgKdeKwinServerDecoration::setRelease(F<void(COrgKdeKwinServerDecoration*)> handler) {
    requests.release = handler;
}

void COrgKdeKwinServerDecoration::setRequestMode(F<void(COrgKdeKwinServerDecoration*, uint32_t)> handler) {
    requests.requestMode = handler;
}

#undef F
