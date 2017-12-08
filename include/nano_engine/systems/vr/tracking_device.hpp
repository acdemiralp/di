#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_HPP_

#include <cstdint>
#include <string>

#include <openvr.h>

#include <nano_engine/systems/vr/hand.hpp>
#include <nano_engine/systems/vr/tracking_device_activity.hpp>
#include <nano_engine/systems/vr/tracking_device_type.hpp>

namespace ne
{
// Important Note: Accesses globals of OpenVR. Do not instantiate until vr::VR_Init has been called.
template <tracking_device_type type>
class tracking_device
{
public:
  explicit tracking_device  (const std::uint32_t index) : index_(index)
  {
    
  }
  tracking_device           (const tracking_device&  that) = default;
  tracking_device           (      tracking_device&& temp) = default;
  virtual ~tracking_device  ()                             = default;
  tracking_device& operator=(const tracking_device&  that) = default;
  tracking_device& operator=(      tracking_device&& temp) = default;

  // IVR System - Tracking
  tracking_device_activity activity           ()                           const
  {
    return static_cast<tracking_device_activity>(vr::VRSystem()->GetTrackedDeviceActivityLevel(index_));
  }
  hand                     hand               ()                           const
  {
    return static_cast<ne::hand>(vr::VRSystem()->GetControllerRoleForTrackedDeviceIndex(index_));
  }
                                                                           
  // IVR System - Property                                                 
  bool                     connected          ()                           const
  {
    return vr::VRSystem()->IsTrackedDeviceConnected(index_);
  }

  // IVR System - Debug
  std::string              send_driver_request(const std::string& request) const
  {
    char raw_response[32768];
    vr::VRSystem()->DriverDebugRequest(index_, request.data(), &raw_response[0], 32768);
    return std::string(raw_response);
  }

  // IVR System - Firmware
  void                     update_firmware    ()                           const
  {
    vr::VRSystem()->PerformFirmwareUpdate(index_);
  }

protected:
  std::uint32_t index_;
};
}

#endif