#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_ACTIVITY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_ACTIVITY_HPP_

#include <openvr.h>

namespace ne
{
enum class tracking_device_activity
{
  unknown             = vr::k_EDeviceActivityLevel_Unknown                ,
  idle                = vr::k_EDeviceActivityLevel_Idle                   ,
  interaction         = vr::k_EDeviceActivityLevel_UserInteraction        ,
  interaction_timeout = vr::k_EDeviceActivityLevel_UserInteraction_Timeout,
  standby             = vr::k_EDeviceActivityLevel_Standby
};
}

#endif