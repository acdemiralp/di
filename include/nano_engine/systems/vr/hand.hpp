#ifndef NANO_ENGINE_SYSTEMS_VR_HAND_HPP_
#define NANO_ENGINE_SYSTEMS_VR_HAND_HPP_

#include <openvr.h>

namespace ne
{
enum class hand
{
  invalid = vr::TrackedControllerRole_Invalid  ,
  left    = vr::TrackedControllerRole_LeftHand ,
  right   = vr::TrackedControllerRole_RightHand
};
}

#endif