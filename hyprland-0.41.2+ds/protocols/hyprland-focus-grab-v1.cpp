// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// hyprland_focus_grab_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2024 outfoxxed
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
*/

#define private public
#define HYPRWAYLAND_SCANNER_NO_INTERFACES
#include "hyprland-focus-grab-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface hyprland_focus_grab_manager_v1_interface;
extern const wl_interface hyprland_focus_grab_v1_interface;
extern const wl_interface wl_surface_interface;

static void _CHyprlandFocusGrabManagerV1CreateGrab(wl_client* client, wl_resource* resource, uint32_t grab) {
    const auto PO = (CHyprlandFocusGrabManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createGrab)
        PO->requests.createGrab(PO, grab);
}

static void _CHyprlandFocusGrabManagerV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CHyprlandFocusGrabManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CHyprlandFocusGrabManagerV1__DestroyListener(wl_listener* l, void* d) {
    CHyprlandFocusGrabManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CHyprlandFocusGrabManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CHyprlandFocusGrabManagerV1VTable[] = {
    (void*)_CHyprlandFocusGrabManagerV1CreateGrab,
    (void*)_CHyprlandFocusGrabManagerV1Destroy,
};
static const wl_interface* _CHyprlandFocusGrabManagerV1CreateGrabTypes[] = {
    &hyprland_focus_grab_v1_interface,
};

static const wl_message _CHyprlandFocusGrabManagerV1Requests[] = {
    { "create_grab", "n", _CHyprlandFocusGrabManagerV1CreateGrabTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

const wl_interface hyprland_focus_grab_manager_v1_interface = {
    "hyprland_focus_grab_manager_v1", 1,
    2, _CHyprlandFocusGrabManagerV1Requests,
    0, nullptr,
};

CHyprlandFocusGrabManagerV1::CHyprlandFocusGrabManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &hyprland_focus_grab_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CHyprlandFocusGrabManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CHyprlandFocusGrabManagerV1VTable, this, nullptr);
}

CHyprlandFocusGrabManagerV1::~CHyprlandFocusGrabManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CHyprlandFocusGrabManagerV1::onDestroyCalled() {
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

void CHyprlandFocusGrabManagerV1::setCreateGrab(F<void(CHyprlandFocusGrabManagerV1*, uint32_t)> handler) {
    requests.createGrab = handler;
}

void CHyprlandFocusGrabManagerV1::setDestroy(F<void(CHyprlandFocusGrabManagerV1*)> handler) {
    requests.destroy = handler;
}

static void _CHyprlandFocusGrabV1AddSurface(wl_client* client, wl_resource* resource, wl_resource* surface) {
    const auto PO = (CHyprlandFocusGrabV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.addSurface)
        PO->requests.addSurface(PO, surface);
}

static void _CHyprlandFocusGrabV1RemoveSurface(wl_client* client, wl_resource* resource, wl_resource* surface) {
    const auto PO = (CHyprlandFocusGrabV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.removeSurface)
        PO->requests.removeSurface(PO, surface);
}

static void _CHyprlandFocusGrabV1Commit(wl_client* client, wl_resource* resource) {
    const auto PO = (CHyprlandFocusGrabV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.commit)
        PO->requests.commit(PO);
}

static void _CHyprlandFocusGrabV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CHyprlandFocusGrabV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CHyprlandFocusGrabV1__DestroyListener(wl_listener* l, void* d) {
    CHyprlandFocusGrabV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CHyprlandFocusGrabV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CHyprlandFocusGrabV1VTable[] = {
    (void*)_CHyprlandFocusGrabV1AddSurface,
    (void*)_CHyprlandFocusGrabV1RemoveSurface,
    (void*)_CHyprlandFocusGrabV1Commit,
    (void*)_CHyprlandFocusGrabV1Destroy,
};

void CHyprlandFocusGrabV1::sendCleared() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CHyprlandFocusGrabV1::sendClearedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}
static const wl_interface* _CHyprlandFocusGrabV1AddSurfaceTypes[] = {
    &wl_surface_interface,
};
static const wl_interface* _CHyprlandFocusGrabV1RemoveSurfaceTypes[] = {
    &wl_surface_interface,
};

static const wl_message _CHyprlandFocusGrabV1Requests[] = {
    { "add_surface", "o", _CHyprlandFocusGrabV1AddSurfaceTypes + 0},
    { "remove_surface", "o", _CHyprlandFocusGrabV1RemoveSurfaceTypes + 0},
    { "commit", "", dummyTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CHyprlandFocusGrabV1Events[] = {
    { "cleared", "", dummyTypes + 0},
};

const wl_interface hyprland_focus_grab_v1_interface = {
    "hyprland_focus_grab_v1", 1,
    4, _CHyprlandFocusGrabV1Requests,
    1, _CHyprlandFocusGrabV1Events,
};

CHyprlandFocusGrabV1::CHyprlandFocusGrabV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &hyprland_focus_grab_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CHyprlandFocusGrabV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CHyprlandFocusGrabV1VTable, this, nullptr);
}

CHyprlandFocusGrabV1::~CHyprlandFocusGrabV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CHyprlandFocusGrabV1::onDestroyCalled() {
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

void CHyprlandFocusGrabV1::setAddSurface(F<void(CHyprlandFocusGrabV1*, wl_resource*)> handler) {
    requests.addSurface = handler;
}

void CHyprlandFocusGrabV1::setRemoveSurface(F<void(CHyprlandFocusGrabV1*, wl_resource*)> handler) {
    requests.removeSurface = handler;
}

void CHyprlandFocusGrabV1::setCommit(F<void(CHyprlandFocusGrabV1*)> handler) {
    requests.commit = handler;
}

void CHyprlandFocusGrabV1::setDestroy(F<void(CHyprlandFocusGrabV1*)> handler) {
    requests.destroy = handler;
}

#undef F
