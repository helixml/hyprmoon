// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// drm

/*
 This protocol's authors' copyright notice is:


    Copyright © 2008-2011 Kristian Høgsberg
    Copyright © 2010-2011 Intel Corporation

    Permission to use, copy, modify, distribute, and sell this
    software and its documentation for any purpose is hereby granted
    without fee, provided that\n the above copyright notice appear in
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
#include "wayland-drm.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wl_drm_interface;
extern const wl_interface wl_buffer_interface;

static void _CWlDrmAuthenticate(wl_client* client, wl_resource* resource, uint32_t id) {
    const auto PO = (CWlDrm*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.authenticate)
        PO->requests.authenticate(PO, id);
}

static void _CWlDrmCreateBuffer(wl_client* client, wl_resource* resource, uint32_t id, uint32_t name, int32_t width, int32_t height, uint32_t stride, uint32_t format) {
    const auto PO = (CWlDrm*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createBuffer)
        PO->requests.createBuffer(PO, id, name, width, height, stride, format);
}

static void _CWlDrmCreatePlanarBuffer(wl_client* client, wl_resource* resource, uint32_t id, uint32_t name, int32_t width, int32_t height, uint32_t format, int32_t offset0, int32_t stride0, int32_t offset1, int32_t stride1, int32_t offset2, int32_t stride2) {
    const auto PO = (CWlDrm*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createPlanarBuffer)
        PO->requests.createPlanarBuffer(PO, id, name, width, height, format, offset0, stride0, offset1, stride1, offset2, stride2);
}

static void _CWlDrmCreatePrimeBuffer(wl_client* client, wl_resource* resource, uint32_t id, int32_t name, int32_t width, int32_t height, uint32_t format, int32_t offset0, int32_t stride0, int32_t offset1, int32_t stride1, int32_t offset2, int32_t stride2) {
    const auto PO = (CWlDrm*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createPrimeBuffer)
        PO->requests.createPrimeBuffer(PO, id, name, width, height, format, offset0, stride0, offset1, stride1, offset2, stride2);
}

static void _CWlDrm__DestroyListener(wl_listener* l, void* d) {
    CWlDrmDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWlDrm* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWlDrmVTable[] = {
    (void*)_CWlDrmAuthenticate,
    (void*)_CWlDrmCreateBuffer,
    (void*)_CWlDrmCreatePlanarBuffer,
    (void*)_CWlDrmCreatePrimeBuffer,
};

void CWlDrm::sendDevice(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name);
}

void CWlDrm::sendFormat(uint32_t format) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, format);
}

void CWlDrm::sendAuthenticated() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlDrm::sendCapabilities(uint32_t value) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, value);
}

void CWlDrm::sendDeviceRaw(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name);
}

void CWlDrm::sendFormatRaw(uint32_t format) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, format);
}

void CWlDrm::sendAuthenticatedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWlDrm::sendCapabilitiesRaw(uint32_t value) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, value);
}
static const wl_interface* _CWlDrmAuthenticateTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDrmCreateBufferTypes[] = {
    &wl_buffer_interface,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDrmCreatePlanarBufferTypes[] = {
    &wl_buffer_interface,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDrmCreatePrimeBufferTypes[] = {
    &wl_buffer_interface,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CWlDrmDeviceTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDrmFormatTypes[] = {
    nullptr,
};
static const wl_interface* _CWlDrmCapabilitiesTypes[] = {
    nullptr,
};

static const wl_message _CWlDrmRequests[] = {
    { "authenticate", "u", _CWlDrmAuthenticateTypes + 0},
    { "create_buffer", "nuiiuu", _CWlDrmCreateBufferTypes + 0},
    { "create_planar_buffer", "nuiiuiiiiii", _CWlDrmCreatePlanarBufferTypes + 0},
    { "create_prime_buffer", "2nhiiuiiiiii", _CWlDrmCreatePrimeBufferTypes + 0},
};

static const wl_message _CWlDrmEvents[] = {
    { "device", "s", _CWlDrmDeviceTypes + 0},
    { "format", "u", _CWlDrmFormatTypes + 0},
    { "authenticated", "", dummyTypes + 0},
    { "capabilities", "u", _CWlDrmCapabilitiesTypes + 0},
};

const wl_interface wl_drm_interface = {
    "wl_drm", 2,
    4, _CWlDrmRequests,
    4, _CWlDrmEvents,
};

CWlDrm::CWlDrm(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wl_drm_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWlDrm__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWlDrmVTable, this, nullptr);
}

CWlDrm::~CWlDrm() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWlDrm::onDestroyCalled() {
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

void CWlDrm::setAuthenticate(F<void(CWlDrm*, uint32_t)> handler) {
    requests.authenticate = handler;
}

void CWlDrm::setCreateBuffer(F<void(CWlDrm*, uint32_t, uint32_t, int32_t, int32_t, uint32_t, uint32_t)> handler) {
    requests.createBuffer = handler;
}

void CWlDrm::setCreatePlanarBuffer(F<void(CWlDrm*, uint32_t, uint32_t, int32_t, int32_t, uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.createPlanarBuffer = handler;
}

void CWlDrm::setCreatePrimeBuffer(F<void(CWlDrm*, uint32_t, int32_t, int32_t, int32_t, uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t)> handler) {
    requests.createPrimeBuffer = handler;
}

#undef F
