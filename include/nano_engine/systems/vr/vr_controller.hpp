#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_HPP_

#include <cstddef>

#include <openvr.h>

#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/systems/vr/tracking_mode.hpp>
#include <nano_engine/systems/vr/vr_controller_state.hpp>

namespace ne
{
class vr_controller : public tracking_device<tracking_device_type::controller>
{
public:
  vr_controller           ()                           = default;
  vr_controller           (const vr_controller&  that) = default;
  vr_controller           (      vr_controller&& temp) = default;
  virtual ~vr_controller  ()                           = default;
  vr_controller& operator=(const vr_controller&  that) = default;
  vr_controller& operator=(      vr_controller&& temp) = default;

  // IVR System - Controller
  vr_controller_state state       (tracking_mode mode)                                    const
  {
    vr::VRControllerState_t controller_state;
    vr::TrackedDevicePose_t controller_pose ;
    vr::VRSystem()->GetControllerStateWithPose(
      static_cast<vr::ETrackingUniverseOrigin>(mode), 
      index_                                        , 
      &controller_state                             , 
      sizeof controller_state                       , 
      &controller_pose                              );
    return vr_controller_state(controller_pose, controller_state);
  }
  void                haptic_pulse(std::size_t axis = 0, std::size_t milliseconds = 1000) const
  {
    vr::VRSystem()->TriggerHapticPulse(index_, static_cast<std::uint32_t>(axis), static_cast<unsigned short>(1000 * milliseconds));
  }
};
}

#endif