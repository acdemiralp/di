#ifndef DI_SYSTEMS_VR_VR_CONTROLLER_HPP_
#define DI_SYSTEMS_VR_VR_CONTROLLER_HPP_

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

#include <openvr.h>

#include <di/systems/vr/hand.hpp>
#include <di/systems/vr/tracking_device.hpp>
#include <di/systems/vr/tracking_mode.hpp>
#include <di/systems/vr/vr_controller_axis.hpp>
#include <di/systems/vr/vr_controller_state.hpp>

namespace di
{
class vr_controller : public tracking_device<tracking_device_type::controller>
{
public:
  explicit vr_controller  (const std::uint32_t& index) : tracking_device<tracking_device_type::controller>(index)
  {
  
  }
  vr_controller           (const vr_controller&  that) = default;
  vr_controller           (      vr_controller&& temp) = default;
  virtual ~vr_controller  ()                           = default;
  vr_controller& operator=(const vr_controller&  that) = default;
  vr_controller& operator=(      vr_controller&& temp) = default;

  // IVR System - Controller
  vr_controller_state state                 (tracking_mode mode = tracking_mode::automatic)                                                          const
  {
    if (mode == tracking_mode::automatic) 
      mode = static_cast<tracking_mode>(vr::VRCompositor()->GetTrackingSpace());

    vr::VRControllerState_t controller_state;
    vr::TrackedDevicePose_t controller_pose ;
    vr::VRSystem()->GetControllerStateWithPose(
      static_cast<vr::ETrackingUniverseOrigin>(mode), 
      index_                                        , 
      &controller_state                             , 
      sizeof controller_state                       , 
      &controller_pose                              );
    return vr_controller_state(controller_pose, controller_state, axis_types());
  }
  void                haptic_pulse          (const std::size_t axis = 0, const std::chrono::milliseconds duration = std::chrono::milliseconds(1000)) const
  {
    vr::VRSystem()->TriggerHapticPulse(index_, static_cast<std::uint32_t>(axis), static_cast<unsigned short>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()));
  }
                                                                   
  // IVR System - Property                                         
  std::string         attached_device_id    () const
  {
    return get_property_string(vr::Prop_AttachedDeviceId_String);
  }
  std::size_t         supported_button_count() const
  {
    return static_cast<std::size_t>(get_property_uint64(vr::Prop_SupportedButtons_Uint64));
  }
  di::hand            hand_hint             () const
  {
    return static_cast<di::hand>(get_property_int(vr::Prop_ControllerRoleHint_Int32));
  }
  
  std::array<vr_controller_axis, vr::k_unControllerStateAxisCount> axis_types() const
  {
    return std::array<vr_controller_axis, vr::k_unControllerStateAxisCount>
    {
      static_cast<vr_controller_axis>(get_property_int(vr::Prop_Axis0Type_Int32)),
      static_cast<vr_controller_axis>(get_property_int(vr::Prop_Axis1Type_Int32)),
      static_cast<vr_controller_axis>(get_property_int(vr::Prop_Axis2Type_Int32)),
      static_cast<vr_controller_axis>(get_property_int(vr::Prop_Axis3Type_Int32)),
      static_cast<vr_controller_axis>(get_property_int(vr::Prop_Axis4Type_Int32))
    };
  }
};
}

#endif