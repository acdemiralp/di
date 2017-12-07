#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_TYPE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_TYPE_HPP_

#include <openvr.h>

namespace ne
{
enum class tracking_device_type
{
  invalid            = vr::TrackedDeviceClass_Invalid          ,
  hmd                = vr::TrackedDeviceClass_HMD              ,
  controller         = vr::TrackedDeviceClass_Controller       ,
  generic_tracker    = vr::TrackedDeviceClass_GenericTracker   ,
  tracking_reference = vr::TrackedDeviceClass_TrackingReference,
  display_redirect   = vr::TrackedDeviceClass_DisplayRedirect
};
}

#endif