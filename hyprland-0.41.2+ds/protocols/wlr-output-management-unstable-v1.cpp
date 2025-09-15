// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// wlr_output_management_unstable_v1

/*
 This protocol's authors' copyright notice is:


    Copyright © 2019 Purism SPC

    Permission to use, copy, modify, distribute, and sell this
    software and its documentation for any purpose is hereby granted
    without fee, provided that the above copyright notice appear in
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
#include "wlr-output-management-unstable-v1.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface zwlr_output_manager_v1_interface;
extern const wl_interface zwlr_output_head_v1_interface;
extern const wl_interface zwlr_output_mode_v1_interface;
extern const wl_interface zwlr_output_configuration_v1_interface;
extern const wl_interface zwlr_output_configuration_head_v1_interface;

static void _CZwlrOutputManagerV1CreateConfiguration(wl_client* client, wl_resource* resource, uint32_t id, uint32_t serial) {
    const auto PO = (CZwlrOutputManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.createConfiguration)
        PO->requests.createConfiguration(PO, id, serial);
}

static void _CZwlrOutputManagerV1Stop(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputManagerV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.stop)
        PO->requests.stop(PO);
}

static void _CZwlrOutputManagerV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputManagerV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputManagerV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputManagerV1VTable[] = {
    (void*)_CZwlrOutputManagerV1CreateConfiguration,
    (void*)_CZwlrOutputManagerV1Stop,
};

void CZwlrOutputManagerV1::sendHead(CZwlrOutputHeadV1* head) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, head ? head->pResource : nullptr);
}

void CZwlrOutputManagerV1::sendDone(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial);
}

void CZwlrOutputManagerV1::sendFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwlrOutputManagerV1::sendHeadRaw(CZwlrOutputHeadV1* head) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, head);
}

void CZwlrOutputManagerV1::sendDoneRaw(uint32_t serial) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, serial);
}

void CZwlrOutputManagerV1::sendFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}
static const wl_interface* _CZwlrOutputManagerV1CreateConfigurationTypes[] = {
    &zwlr_output_configuration_v1_interface,
    nullptr,
};
static const wl_interface* _CZwlrOutputManagerV1HeadTypes[] = {
    &zwlr_output_head_v1_interface,
};
static const wl_interface* _CZwlrOutputManagerV1DoneTypes[] = {
    nullptr,
};

static const wl_message _CZwlrOutputManagerV1Requests[] = {
    { "create_configuration", "nu", _CZwlrOutputManagerV1CreateConfigurationTypes + 0},
    { "stop", "", dummyTypes + 0},
};

static const wl_message _CZwlrOutputManagerV1Events[] = {
    { "head", "n", _CZwlrOutputManagerV1HeadTypes + 0},
    { "done", "u", _CZwlrOutputManagerV1DoneTypes + 0},
    { "finished", "", dummyTypes + 0},
};

const wl_interface zwlr_output_manager_v1_interface = {
    "zwlr_output_manager_v1", 4,
    2, _CZwlrOutputManagerV1Requests,
    3, _CZwlrOutputManagerV1Events,
};

CZwlrOutputManagerV1::CZwlrOutputManagerV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_manager_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputManagerV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputManagerV1VTable, this, nullptr);
}

CZwlrOutputManagerV1::~CZwlrOutputManagerV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputManagerV1::onDestroyCalled() {
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

void CZwlrOutputManagerV1::setCreateConfiguration(F<void(CZwlrOutputManagerV1*, uint32_t, uint32_t)> handler) {
    requests.createConfiguration = handler;
}

void CZwlrOutputManagerV1::setStop(F<void(CZwlrOutputManagerV1*)> handler) {
    requests.stop = handler;
}

static void _CZwlrOutputHeadV1Release(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CZwlrOutputHeadV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputHeadV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputHeadV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputHeadV1VTable[] = {
    (void*)_CZwlrOutputHeadV1Release,
};

void CZwlrOutputHeadV1::sendName(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name);
}

void CZwlrOutputHeadV1::sendDescription(const char* description) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, description);
}

void CZwlrOutputHeadV1::sendPhysicalSize(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, width, height);
}

void CZwlrOutputHeadV1::sendMode(CZwlrOutputModeV1* mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, mode ? mode->pResource : nullptr);
}

void CZwlrOutputHeadV1::sendEnabled(int32_t enabled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, enabled);
}

void CZwlrOutputHeadV1::sendCurrentMode(CZwlrOutputModeV1* mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, mode ? mode->pResource : nullptr);
}

void CZwlrOutputHeadV1::sendPosition(int32_t x, int32_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, x, y);
}

void CZwlrOutputHeadV1::sendTransform(int32_t transform) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7, transform);
}

void CZwlrOutputHeadV1::sendScale(wl_fixed_t scale) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 8, scale);
}

void CZwlrOutputHeadV1::sendFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 9);
}

void CZwlrOutputHeadV1::sendMake(const char* make) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 10, make);
}

void CZwlrOutputHeadV1::sendModel(const char* model) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 11, model);
}

void CZwlrOutputHeadV1::sendSerialNumber(const char* serial_number) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 12, serial_number);
}

void CZwlrOutputHeadV1::sendAdaptiveSync(zwlrOutputHeadV1AdaptiveSyncState state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 13, state);
}

void CZwlrOutputHeadV1::sendNameRaw(const char* name) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, name);
}

void CZwlrOutputHeadV1::sendDescriptionRaw(const char* description) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, description);
}

void CZwlrOutputHeadV1::sendPhysicalSizeRaw(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2, width, height);
}

void CZwlrOutputHeadV1::sendModeRaw(CZwlrOutputModeV1* mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3, mode);
}

void CZwlrOutputHeadV1::sendEnabledRaw(int32_t enabled) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 4, enabled);
}

void CZwlrOutputHeadV1::sendCurrentModeRaw(wl_resource* mode) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 5, mode);
}

void CZwlrOutputHeadV1::sendPositionRaw(int32_t x, int32_t y) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 6, x, y);
}

void CZwlrOutputHeadV1::sendTransformRaw(int32_t transform) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 7, transform);
}

void CZwlrOutputHeadV1::sendScaleRaw(wl_fixed_t scale) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 8, scale);
}

void CZwlrOutputHeadV1::sendFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 9);
}

void CZwlrOutputHeadV1::sendMakeRaw(const char* make) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 10, make);
}

void CZwlrOutputHeadV1::sendModelRaw(const char* model) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 11, model);
}

void CZwlrOutputHeadV1::sendSerialNumberRaw(const char* serial_number) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 12, serial_number);
}

void CZwlrOutputHeadV1::sendAdaptiveSyncRaw(zwlrOutputHeadV1AdaptiveSyncState state) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 13, state);
}
static const wl_interface* _CZwlrOutputHeadV1NameTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1DescriptionTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1PhysicalSizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1ModeTypes[] = {
    &zwlr_output_mode_v1_interface,
};
static const wl_interface* _CZwlrOutputHeadV1EnabledTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1CurrentModeTypes[] = {
    &zwlr_output_mode_v1_interface,
};
static const wl_interface* _CZwlrOutputHeadV1PositionTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1TransformTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1ScaleTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1MakeTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1ModelTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1SerialNumberTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputHeadV1AdaptiveSyncTypes[] = {
    nullptr,
};

static const wl_message _CZwlrOutputHeadV1Requests[] = {
    { "release", "3", dummyTypes + 0},
};

static const wl_message _CZwlrOutputHeadV1Events[] = {
    { "name", "s", _CZwlrOutputHeadV1NameTypes + 0},
    { "description", "s", _CZwlrOutputHeadV1DescriptionTypes + 0},
    { "physical_size", "ii", _CZwlrOutputHeadV1PhysicalSizeTypes + 0},
    { "mode", "n", _CZwlrOutputHeadV1ModeTypes + 0},
    { "enabled", "i", _CZwlrOutputHeadV1EnabledTypes + 0},
    { "current_mode", "o", _CZwlrOutputHeadV1CurrentModeTypes + 0},
    { "position", "ii", _CZwlrOutputHeadV1PositionTypes + 0},
    { "transform", "i", _CZwlrOutputHeadV1TransformTypes + 0},
    { "scale", "f", _CZwlrOutputHeadV1ScaleTypes + 0},
    { "finished", "", dummyTypes + 0},
    { "make", "2s", _CZwlrOutputHeadV1MakeTypes + 0},
    { "model", "2s", _CZwlrOutputHeadV1ModelTypes + 0},
    { "serial_number", "2s", _CZwlrOutputHeadV1SerialNumberTypes + 0},
    { "adaptive_sync", "4u", _CZwlrOutputHeadV1AdaptiveSyncTypes + 0},
};

const wl_interface zwlr_output_head_v1_interface = {
    "zwlr_output_head_v1", 4,
    1, _CZwlrOutputHeadV1Requests,
    14, _CZwlrOutputHeadV1Events,
};

CZwlrOutputHeadV1::CZwlrOutputHeadV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_head_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputHeadV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputHeadV1VTable, this, nullptr);
}

CZwlrOutputHeadV1::~CZwlrOutputHeadV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputHeadV1::onDestroyCalled() {
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

void CZwlrOutputHeadV1::setRelease(F<void(CZwlrOutputHeadV1*)> handler) {
    requests.release = handler;
}

static void _CZwlrOutputModeV1Release(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputModeV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.release)
        PO->requests.release(PO);
}

static void _CZwlrOutputModeV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputModeV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputModeV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputModeV1VTable[] = {
    (void*)_CZwlrOutputModeV1Release,
};

void CZwlrOutputModeV1::sendSize(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, width, height);
}

void CZwlrOutputModeV1::sendRefresh(int32_t refresh) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, refresh);
}

void CZwlrOutputModeV1::sendPreferred() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwlrOutputModeV1::sendFinished() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}

void CZwlrOutputModeV1::sendSizeRaw(int32_t width, int32_t height) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, width, height);
}

void CZwlrOutputModeV1::sendRefreshRaw(int32_t refresh) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, refresh);
}

void CZwlrOutputModeV1::sendPreferredRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwlrOutputModeV1::sendFinishedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 3);
}
static const wl_interface* _CZwlrOutputModeV1SizeTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrOutputModeV1RefreshTypes[] = {
    nullptr,
};

static const wl_message _CZwlrOutputModeV1Requests[] = {
    { "release", "3", dummyTypes + 0},
};

static const wl_message _CZwlrOutputModeV1Events[] = {
    { "size", "ii", _CZwlrOutputModeV1SizeTypes + 0},
    { "refresh", "i", _CZwlrOutputModeV1RefreshTypes + 0},
    { "preferred", "", dummyTypes + 0},
    { "finished", "", dummyTypes + 0},
};

const wl_interface zwlr_output_mode_v1_interface = {
    "zwlr_output_mode_v1", 3,
    1, _CZwlrOutputModeV1Requests,
    4, _CZwlrOutputModeV1Events,
};

CZwlrOutputModeV1::CZwlrOutputModeV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_mode_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputModeV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputModeV1VTable, this, nullptr);
}

CZwlrOutputModeV1::~CZwlrOutputModeV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputModeV1::onDestroyCalled() {
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

void CZwlrOutputModeV1::setRelease(F<void(CZwlrOutputModeV1*)> handler) {
    requests.release = handler;
}

static void _CZwlrOutputConfigurationV1EnableHead(wl_client* client, wl_resource* resource, uint32_t id, wl_resource* head) {
    const auto PO = (CZwlrOutputConfigurationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.enableHead)
        PO->requests.enableHead(PO, id, head);
}

static void _CZwlrOutputConfigurationV1DisableHead(wl_client* client, wl_resource* resource, wl_resource* head) {
    const auto PO = (CZwlrOutputConfigurationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.disableHead)
        PO->requests.disableHead(PO, head);
}

static void _CZwlrOutputConfigurationV1Apply(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputConfigurationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.apply)
        PO->requests.apply(PO);
}

static void _CZwlrOutputConfigurationV1Test(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputConfigurationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.test)
        PO->requests.test(PO);
}

static void _CZwlrOutputConfigurationV1Destroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CZwlrOutputConfigurationV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CZwlrOutputConfigurationV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputConfigurationV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputConfigurationV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputConfigurationV1VTable[] = {
    (void*)_CZwlrOutputConfigurationV1EnableHead,
    (void*)_CZwlrOutputConfigurationV1DisableHead,
    (void*)_CZwlrOutputConfigurationV1Apply,
    (void*)_CZwlrOutputConfigurationV1Test,
    (void*)_CZwlrOutputConfigurationV1Destroy,
};

void CZwlrOutputConfigurationV1::sendSucceeded() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CZwlrOutputConfigurationV1::sendFailed() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwlrOutputConfigurationV1::sendCancelled() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CZwlrOutputConfigurationV1::sendSucceededRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0);
}

void CZwlrOutputConfigurationV1::sendFailedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1);
}

void CZwlrOutputConfigurationV1::sendCancelledRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}
static const wl_interface* _CZwlrOutputConfigurationV1EnableHeadTypes[] = {
    &zwlr_output_configuration_head_v1_interface,
    &zwlr_output_head_v1_interface,
};
static const wl_interface* _CZwlrOutputConfigurationV1DisableHeadTypes[] = {
    &zwlr_output_head_v1_interface,
};

static const wl_message _CZwlrOutputConfigurationV1Requests[] = {
    { "enable_head", "no", _CZwlrOutputConfigurationV1EnableHeadTypes + 0},
    { "disable_head", "o", _CZwlrOutputConfigurationV1DisableHeadTypes + 0},
    { "apply", "", dummyTypes + 0},
    { "test", "", dummyTypes + 0},
    { "destroy", "", dummyTypes + 0},
};

static const wl_message _CZwlrOutputConfigurationV1Events[] = {
    { "succeeded", "", dummyTypes + 0},
    { "failed", "", dummyTypes + 0},
    { "cancelled", "", dummyTypes + 0},
};

const wl_interface zwlr_output_configuration_v1_interface = {
    "zwlr_output_configuration_v1", 4,
    5, _CZwlrOutputConfigurationV1Requests,
    3, _CZwlrOutputConfigurationV1Events,
};

CZwlrOutputConfigurationV1::CZwlrOutputConfigurationV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_configuration_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputConfigurationV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputConfigurationV1VTable, this, nullptr);
}

CZwlrOutputConfigurationV1::~CZwlrOutputConfigurationV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputConfigurationV1::onDestroyCalled() {
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

void CZwlrOutputConfigurationV1::setEnableHead(F<void(CZwlrOutputConfigurationV1*, uint32_t, wl_resource*)> handler) {
    requests.enableHead = handler;
}

void CZwlrOutputConfigurationV1::setDisableHead(F<void(CZwlrOutputConfigurationV1*, wl_resource*)> handler) {
    requests.disableHead = handler;
}

void CZwlrOutputConfigurationV1::setApply(F<void(CZwlrOutputConfigurationV1*)> handler) {
    requests.apply = handler;
}

void CZwlrOutputConfigurationV1::setTest(F<void(CZwlrOutputConfigurationV1*)> handler) {
    requests.test = handler;
}

void CZwlrOutputConfigurationV1::setDestroy(F<void(CZwlrOutputConfigurationV1*)> handler) {
    requests.destroy = handler;
}

static void _CZwlrOutputConfigurationHeadV1SetMode(wl_client* client, wl_resource* resource, wl_resource* mode) {
    const auto PO = (CZwlrOutputConfigurationHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setMode)
        PO->requests.setMode(PO, mode);
}

static void _CZwlrOutputConfigurationHeadV1SetCustomMode(wl_client* client, wl_resource* resource, int32_t width, int32_t height, int32_t refresh) {
    const auto PO = (CZwlrOutputConfigurationHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setCustomMode)
        PO->requests.setCustomMode(PO, width, height, refresh);
}

static void _CZwlrOutputConfigurationHeadV1SetPosition(wl_client* client, wl_resource* resource, int32_t x, int32_t y) {
    const auto PO = (CZwlrOutputConfigurationHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setPosition)
        PO->requests.setPosition(PO, x, y);
}

static void _CZwlrOutputConfigurationHeadV1SetTransform(wl_client* client, wl_resource* resource, int32_t transform) {
    const auto PO = (CZwlrOutputConfigurationHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setTransform)
        PO->requests.setTransform(PO, transform);
}

static void _CZwlrOutputConfigurationHeadV1SetScale(wl_client* client, wl_resource* resource, wl_fixed_t scale) {
    const auto PO = (CZwlrOutputConfigurationHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setScale)
        PO->requests.setScale(PO, scale);
}

static void _CZwlrOutputConfigurationHeadV1SetAdaptiveSync(wl_client* client, wl_resource* resource, uint32_t state) {
    const auto PO = (CZwlrOutputConfigurationHeadV1*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.setAdaptiveSync)
        PO->requests.setAdaptiveSync(PO, state);
}

static void _CZwlrOutputConfigurationHeadV1__DestroyListener(wl_listener* l, void* d) {
    CZwlrOutputConfigurationHeadV1DestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CZwlrOutputConfigurationHeadV1* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CZwlrOutputConfigurationHeadV1VTable[] = {
    (void*)_CZwlrOutputConfigurationHeadV1SetMode,
    (void*)_CZwlrOutputConfigurationHeadV1SetCustomMode,
    (void*)_CZwlrOutputConfigurationHeadV1SetPosition,
    (void*)_CZwlrOutputConfigurationHeadV1SetTransform,
    (void*)_CZwlrOutputConfigurationHeadV1SetScale,
    (void*)_CZwlrOutputConfigurationHeadV1SetAdaptiveSync,
};
static const wl_interface* _CZwlrOutputConfigurationHeadV1SetModeTypes[] = {
    &zwlr_output_mode_v1_interface,
};
static const wl_interface* _CZwlrOutputConfigurationHeadV1SetCustomModeTypes[] = {
    nullptr,
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrOutputConfigurationHeadV1SetPositionTypes[] = {
    nullptr,
    nullptr,
};
static const wl_interface* _CZwlrOutputConfigurationHeadV1SetTransformTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputConfigurationHeadV1SetScaleTypes[] = {
    nullptr,
};
static const wl_interface* _CZwlrOutputConfigurationHeadV1SetAdaptiveSyncTypes[] = {
    nullptr,
};

static const wl_message _CZwlrOutputConfigurationHeadV1Requests[] = {
    { "set_mode", "o", _CZwlrOutputConfigurationHeadV1SetModeTypes + 0},
    { "set_custom_mode", "iii", _CZwlrOutputConfigurationHeadV1SetCustomModeTypes + 0},
    { "set_position", "ii", _CZwlrOutputConfigurationHeadV1SetPositionTypes + 0},
    { "set_transform", "i", _CZwlrOutputConfigurationHeadV1SetTransformTypes + 0},
    { "set_scale", "f", _CZwlrOutputConfigurationHeadV1SetScaleTypes + 0},
    { "set_adaptive_sync", "4u", _CZwlrOutputConfigurationHeadV1SetAdaptiveSyncTypes + 0},
};

const wl_interface zwlr_output_configuration_head_v1_interface = {
    "zwlr_output_configuration_head_v1", 4,
    6, _CZwlrOutputConfigurationHeadV1Requests,
    0, nullptr,
};

CZwlrOutputConfigurationHeadV1::CZwlrOutputConfigurationHeadV1(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &zwlr_output_configuration_head_v1_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CZwlrOutputConfigurationHeadV1__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CZwlrOutputConfigurationHeadV1VTable, this, nullptr);
}

CZwlrOutputConfigurationHeadV1::~CZwlrOutputConfigurationHeadV1() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CZwlrOutputConfigurationHeadV1::onDestroyCalled() {
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

void CZwlrOutputConfigurationHeadV1::setSetMode(F<void(CZwlrOutputConfigurationHeadV1*, wl_resource*)> handler) {
    requests.setMode = handler;
}

void CZwlrOutputConfigurationHeadV1::setSetCustomMode(F<void(CZwlrOutputConfigurationHeadV1*, int32_t, int32_t, int32_t)> handler) {
    requests.setCustomMode = handler;
}

void CZwlrOutputConfigurationHeadV1::setSetPosition(F<void(CZwlrOutputConfigurationHeadV1*, int32_t, int32_t)> handler) {
    requests.setPosition = handler;
}

void CZwlrOutputConfigurationHeadV1::setSetTransform(F<void(CZwlrOutputConfigurationHeadV1*, int32_t)> handler) {
    requests.setTransform = handler;
}

void CZwlrOutputConfigurationHeadV1::setSetScale(F<void(CZwlrOutputConfigurationHeadV1*, wl_fixed_t)> handler) {
    requests.setScale = handler;
}

void CZwlrOutputConfigurationHeadV1::setSetAdaptiveSync(F<void(CZwlrOutputConfigurationHeadV1*, uint32_t)> handler) {
    requests.setAdaptiveSync = handler;
}

#undef F
