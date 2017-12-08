#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_

#include <array>
#include <map>

#include <openvr.h>

#include <nano_engine/systems/vr/tracking_device_pose.hpp>
#include <nano_engine/systems/vr/vr_controller_axis.hpp>
#include <nano_engine/systems/vr/vr_controller_button.hpp>
#include <nano_engine/systems/vr/vr_controller_button_state.hpp>

namespace ne
{
struct vr_controller_state : tracking_device_pose
{
  std::map<vr_controller_axis  , std::array<float, 2>>       axes   ;
  std::map<vr_controller_button, vr_controller_button_state> buttons;
};
}

#endif