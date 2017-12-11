#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_

#include <algorithm>
#include <array>
#include <map>

#include <nano_engine/systems/vr/tracking_device_pose.hpp>
#include <nano_engine/systems/vr/vr_controller_button.hpp>
#include <nano_engine/systems/vr/vr_controller_button_state.hpp>

namespace ne
{
struct vr_controller_state : tracking_device_pose
{
  vr_controller_state           (const vr::TrackedDevicePose_t& native_pose, const vr::VRControllerState_t& native_state) 
  : tracking_device_pose(native_pose)
  {
    const auto native_axes = native_state.rAxis;
    for (auto i = 0; i < axes.size(); ++i)
      axes[i] = {native_axes[i].x, native_axes[i].y};

    const auto pressed = native_state.ulButtonPressed;
    const auto touched = native_state.ulButtonTouched;
    for(auto& button : vr_controller_buttons)
    {
      const auto mask = vr::ButtonMaskFromId(static_cast<vr::EVRButtonId>(button));
      buttons[button] = 
        pressed & mask ? vr_controller_button_state::pressed : 
        touched & mask ? vr_controller_button_state::touched : 
                         vr_controller_button_state::released;
    }
  }
  vr_controller_state           (const vr_controller_state&  that) = default;
  vr_controller_state           (      vr_controller_state&& temp) = default;
  virtual ~vr_controller_state  ()                                 = default;
  vr_controller_state& operator=(const vr_controller_state&  that) = default;
  vr_controller_state& operator=(      vr_controller_state&& temp) = default;

  std::array<std::array<float, 2>, vr::k_unControllerStateAxisCount> axes   ;
  std::map<vr_controller_button, vr_controller_button_state>         buttons;
};
}

#endif