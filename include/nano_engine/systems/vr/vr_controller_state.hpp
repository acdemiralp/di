#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_

#include <algorithm>
#include <array>
#include <map>

#include <nano_engine/systems/vr/tracking_device_pose.hpp>
#include <nano_engine/systems/vr/vr_controller_axis.hpp>
#include <nano_engine/systems/vr/vr_controller_button.hpp>
#include <nano_engine/systems/vr/vr_controller_button_state.hpp>

namespace ne
{
struct vr_controller_state : tracking_device_pose
{
  vr_controller_state           (const vr::TrackedDevicePose_t& native_pose, const vr::VRControllerState_t& native_state) 
  : tracking_device_pose(native_pose)
  {
    auto pressed = native_state.ulButtonPressed;
    auto touched = native_state.ulButtonTouched;
    auto axes    = native_state.rAxis;
    // TODO! vr::ButtonMaskFromId(static_cast<vr::EVRButtonId>());
  }
  vr_controller_state           (const vr_controller_state&  that) = default;
  vr_controller_state           (      vr_controller_state&& temp) = default;
  virtual ~vr_controller_state  ()                                 = default;
  vr_controller_state& operator=(const vr_controller_state&  that) = default;
  vr_controller_state& operator=(      vr_controller_state&& temp) = default;

  std::map<vr_controller_axis  , std::array<float, 2>>       axes   ;
  std::map<vr_controller_button, vr_controller_button_state> buttons;
};
}

#endif