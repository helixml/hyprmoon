// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// viewporter

/*
 This protocol's authors' copyright notice is:


    Copyright © 2013-2016 Collabora, Ltd.

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
#include "viewporter.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wp_viewporter_interface;
extern const wl_interface wp_viewport_interface;
extern const wl_interface wl_surface_interface;

static void _CWpViewporterDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpViewporter*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpViewporterGetViewport(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* surface) {
    const auto PO = (CWpViewporter*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.getViewport)
        PO->requests.getViewport(PO, id, surface);
}

static void _CWpViewporter__DestroyListener(wl_listener* l, void* d) {
    CWpViewporterDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpViewporter* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpViewporterVTable[] = {
    (void*)_CWpViewporterDestroy,
    (void*)_CWpViewporterGetViewport,
};
static const wl_interface* _CWpViewporterGetViewportTypes[] = {
    &wp_viewport_interface,
    &wl_surface_interface,
};

static const wl_message _CWpViewporterRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "get_viewport", "no", _CWpViewporterGetViewportTypes + 0},
};

const wl_interface wp_viewporter_interface = {
    "wp_viewporter", 1,
    2, _CWpViewporterRequests,
    0, nullptr,
};

CWpViewporter::CWpViewporter(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_viewporter_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpViewporter__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpViewporterVTable, this, nullptr);
}

CWpViewporter::~CWpViewporter() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpViewporter::onDestroyCalled() {
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

void CWpViewporter::setDestroy(F<void(CWpViewporter*)> handler) {
    requests.destroy = handler;
}

void CWpViewporter::setGetViewport(F<void(CWpViewporter*, uint32_t, wl_resource*)> handler) {
    requests.getViewport = handler;
}

static void _CWpViewportDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpViewport*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpViewportSetSource(wl_client* client, wl_resource* resource, wl_fixed_t x, wl_fixed_t y, wl_fixed_t width, wl_fixed_t height) {
    const auto PO = (CWpViewport*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setSource)
        PO->requests.setSource(PO, x, y, width, height);
}

static void _CWpViewportSetDestination(wl_client* client, wl_resource* resource, int32_t width, int32_t height) {
    const auto PO = (CWpViewport*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setDestination)
        PO->requests.setDestination(PO, width, height);
}

static void _CWpViewport__DestroyListener(wl_listener* l, void* d) {
    CWpViewportDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpViewport* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpViewportVTable[] = {
    (void*)_CWpViewportDestroy,
    (void*)_CWpViewportSetSource,
    (void*)_CWpViewportSetDestination,
};
static const wl_interface* _CWpViewportSetSourceTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWpViewportSetDestinationTypes[] = {
    nullptr,
    nullptr,
};

static const wl_message _CWpViewportRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "set_source", "ffff", _CWpViewportSetSourceTypes + 0},
    { "set_destination", "ii", _CWpViewportSetDestinationTypes + 0},
};

const wl_interface wp_viewport_interface = {
    "wp_viewport", 1,
    3, _CWpViewportRequests,
    0, nullptr,
};

CWpViewport::CWpViewport(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_viewport_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpViewport__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpViewportVTable, this, nullptr);
}

CWpViewport::~CWpViewport() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpViewport::onDestroyCalled() {
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

void CWpViewport::setDestroy(F<void(CWpViewport*)> handler) {
    requests.destroy = handler;
}

void CWpViewport::setSetSource(F<void(CWpViewport*, wl_fixed_t, wl_fixed_t, wl_fixed_t, wl_fixed_t)> handler) {
    requests.setSource = handler;
}

void CWpViewport::setSetDestination(F<void(CWpViewport*, int32_t, int32_t)> handler) {
    requests.setDestination = handler;
}

#undef F
