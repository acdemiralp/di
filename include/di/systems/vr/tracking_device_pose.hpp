#ifndef DI_SYSTEMS_VR_TRACKING_DEVICE_POSE_HPP_
#define DI_SYSTEMS_VR_TRACKING_DEVICE_POSE_HPP_

#include <algorithm>
#include <array>

#include <di/systems/vr/tracking_device_state.hpp>

namespace di
{
struct tracking_device_pose
{
  explicit tracking_device_pose  (const vr::TrackedDevicePose_t& native)
  : state    (static_cast<tracking_device_state>(native.eTrackingResult))
  , valid    (native.bPoseIsValid      )
  , connected(native.bDeviceIsConnected)
  {
    std::copy(&native.mDeviceToAbsoluteTracking.m[0][0], &native.mDeviceToAbsoluteTracking.m[0][0] + 12, absolute_matrix .begin());
    std::copy(&native.vVelocity                .v[0]   , &native.vVelocity                .v[0]    + 3 , velocity        .begin());
    std::copy(&native.vAngularVelocity         .v[0]   , &native.vAngularVelocity         .v[0]    + 3 , angular_velocity.begin());
  }
  tracking_device_pose           (const tracking_device_pose&  that) = default;
  tracking_device_pose           (      tracking_device_pose&& temp) = default;
  virtual ~tracking_device_pose  ()                                  = default;
  tracking_device_pose& operator=(const tracking_device_pose&  that) = default;
  tracking_device_pose& operator=(      tracking_device_pose&& temp) = default;

  std::array<float, 12> absolute_matrix ;
  std::array<float, 3>  velocity        ;
  std::array<float, 3>  angular_velocity;
  tracking_device_state state           ;
  bool                  valid           ;
  bool                  connected       ;
};
}

#endif