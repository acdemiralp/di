#ifndef DI_SYSTEMS_VR_VR_CONTROLLER_AXIS_HPP_
#define DI_SYSTEMS_VR_VR_CONTROLLER_AXIS_HPP_

#include <openvr.h>

namespace di
{
enum class vr_controller_axis
{
  none      = vr::k_eControllerAxis_None    ,
  track_pad = vr::k_eControllerAxis_TrackPad,
  joystick  = vr::k_eControllerAxis_Joystick,
  trigger   = vr::k_eControllerAxis_Trigger
};
}

#endif