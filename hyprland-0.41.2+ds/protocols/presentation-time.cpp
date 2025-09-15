// Generated with hyprwayland-scanner 0.4.2. Made with vaxry's keyboard and ❤️.
// presentation_time

/*
 This protocol's authors' copyright notice is:


    Copyright © 2013-2014 Collabora, Ltd.

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
#include "presentation-time.hpp"
#undef private
#define F std::function

static const wl_interface* dummyTypes[] = { nullptr };

// Reference all other interfaces.
// The reason why this is in snake is to
// be able to cooperate with existing
// wayland_scanner interfaces (they are interop)
extern const wl_interface wp_presentation_interface;
extern const wl_interface wp_presentation_feedback_interface;
extern const wl_interface wl_surface_interface;
extern const wl_interface wl_output_interface;

static void _CWpPresentationDestroy(wl_client* client, wl_resource* resource) {
    const auto PO = (CWpPresentation*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.destroy)
        PO->requests.destroy(PO);
}

static void _CWpPresentationFeedback(wl_client* client, wl_resource* resource, wl_resource* surface, uint32_t callback) {
    const auto PO = (CWpPresentation*)wl_resource_get_user_data(resource);
    if (PO && PO->requests.feedback)
        PO->requests.feedback(PO, surface, callback);
}

static void _CWpPresentation__DestroyListener(wl_listener* l, void* d) {
    CWpPresentationDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpPresentation* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpPresentationVTable[] = {
    (void*)_CWpPresentationDestroy,
    (void*)_CWpPresentationFeedback,
};

void CWpPresentation::sendClockId(uint32_t clk_id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, clk_id);
}

void CWpPresentation::sendClockIdRaw(uint32_t clk_id) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, clk_id);
}
static const wl_interface* _CWpPresentationFeedbackTypes[] = {
    &wl_surface_interface,
    &wp_presentation_feedback_interface,
};
static const wl_interface* _CWpPresentationClockIdTypes[] = {
    nullptr,
};

static const wl_message _CWpPresentationRequests[] = {
    { "destroy", "", dummyTypes + 0},
    { "feedback", "on", _CWpPresentationFeedbackTypes + 0},
};

static const wl_message _CWpPresentationEvents[] = {
    { "clock_id", "u", _CWpPresentationClockIdTypes + 0},
};

const wl_interface wp_presentation_interface = {
    "wp_presentation", 2,
    2, _CWpPresentationRequests,
    1, _CWpPresentationEvents,
};

CWpPresentation::CWpPresentation(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_presentation_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpPresentation__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpPresentationVTable, this, nullptr);
}

CWpPresentation::~CWpPresentation() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpPresentation::onDestroyCalled() {
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

void CWpPresentation::setDestroy(F<void(CWpPresentation*)> handler) {
    requests.destroy = handler;
}

void CWpPresentation::setFeedback(F<void(CWpPresentation*, wl_resource*, uint32_t)> handler) {
    requests.feedback = handler;
}

static void _CWpPresentationFeedback__DestroyListener(wl_listener* l, void* d) {
    CWpPresentationFeedbackDestroyWrapper *wrap = wl_container_of(l, wrap, listener);
    CWpPresentationFeedback* pResource = wrap->parent;
    pResource->onDestroyCalled();
}

static const void* _CWpPresentationFeedbackVTable[] = {
};

void CWpPresentationFeedback::sendSyncOutput(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, output);
}

void CWpPresentationFeedback::sendPresented(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec, uint32_t refresh, uint32_t seq_hi, uint32_t seq_lo, wpPresentationFeedbackKind flags) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, tv_sec_hi, tv_sec_lo, tv_nsec, refresh, seq_hi, seq_lo, flags);
}

void CWpPresentationFeedback::sendDiscarded() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}

void CWpPresentationFeedback::sendSyncOutputRaw(wl_resource* output) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 0, output);
}

void CWpPresentationFeedback::sendPresentedRaw(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec, uint32_t refresh, uint32_t seq_hi, uint32_t seq_lo, wpPresentationFeedbackKind flags) {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 1, tv_sec_hi, tv_sec_lo, tv_nsec, refresh, seq_hi, seq_lo, flags);
}

void CWpPresentationFeedback::sendDiscardedRaw() {
    if (!pResource)
        return;
    wl_resource_post_event(pResource, 2);
}
static const wl_interface* _CWpPresentationFeedbackSyncOutputTypes[] = {
    &wl_output_interface,
};
static const wl_interface* _CWpPresentationFeedbackPresentedTypes[] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const wl_message _CWpPresentationFeedbackEvents[] = {
    { "sync_output", "o", _CWpPresentationFeedbackSyncOutputTypes + 0},
    { "presented", "uuuuuuu", _CWpPresentationFeedbackPresentedTypes + 0},
    { "discarded", "", dummyTypes + 0},
};

const wl_interface wp_presentation_feedback_interface = {
    "wp_presentation_feedback", 2,
    0, nullptr,
    3, _CWpPresentationFeedbackEvents,
};

CWpPresentationFeedback::CWpPresentationFeedback(wl_client* client, uint32_t version, uint32_t id) {
    pResource = wl_resource_create(client, &wp_presentation_feedback_interface, version, id);

    if (!pResource)
        return;

    wl_resource_set_user_data(pResource, this);
    wl_list_init(&resourceDestroyListener.listener.link);
    resourceDestroyListener.listener.notify = _CWpPresentationFeedback__DestroyListener;
    resourceDestroyListener.parent = this;
    wl_resource_add_destroy_listener(pResource, &resourceDestroyListener.listener);

    wl_resource_set_implementation(pResource, _CWpPresentationFeedbackVTable, this, nullptr);
}

CWpPresentationFeedback::~CWpPresentationFeedback() {
    wl_list_remove(&resourceDestroyListener.listener.link);
    wl_list_init(&resourceDestroyListener.listener.link);

    // if we still own the wayland resource,
    // it means we need to destroy it.
    if (pResource && wl_resource_get_user_data(pResource) == this) {
        wl_resource_set_user_data(pResource, nullptr);
        wl_resource_destroy(pResource);
    }
}

void CWpPresentationFeedback::onDestroyCalled() {
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

#undef F
