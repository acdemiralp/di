#ifndef DI_SYSTEMS_VR_HAND_HPP_
#define DI_SYSTEMS_VR_HAND_HPP_

#include <openvr.h>

namespace di
{
enum class hand
{
  invalid = vr::TrackedControllerRole_Invalid  ,
  left    = vr::TrackedControllerRole_LeftHand ,
  right   = vr::TrackedControllerRole_RightHand
};
}

#endif