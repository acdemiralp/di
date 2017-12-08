#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_POSE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_POSE_HPP_

#include <array>

#include <openvr.h>

#include <nano_engine/systems/vr/tracking_device_state.hpp>

namespace ne
{
struct tracking_device_pose
{
  std::array<float, 12> absolute_matrix ;
  std::array<float, 3>  velocity        ;
  std::array<float, 3>  angular_velocity;
  tracking_device_state state           ;
  bool                  valid           ;
  bool                  connected       ;

};
}

#endif