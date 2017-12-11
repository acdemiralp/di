#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_HPP_

#include <algorithm>
#include <cstdint>
#include <string>

#include <boost/optional.hpp>
#include <openvr.h>

#include <nano_engine/systems/vr/hand.hpp>
#include <nano_engine/systems/vr/tracking_device_activity.hpp>
#include <nano_engine/systems/vr/tracking_device_pose.hpp>
#include <nano_engine/systems/vr/tracking_device_type.hpp>

namespace ne
{
// Important Note: Accesses globals of OpenVR. Do not instantiate until vr::VR_Init has been called.
template <tracking_device_type type>
class tracking_device
{
public:
  explicit tracking_device  (const std::uint32_t& index) : index_(index), pose_(boost::none)
  {
    
  }
  tracking_device           (const tracking_device&  that) = default;
  tracking_device           (      tracking_device&& temp) = default;
  virtual ~tracking_device  ()                             = default;
  tracking_device& operator=(const tracking_device&  that) = default;
  tracking_device& operator=(      tracking_device&& temp) = default;
  
  // IVR System - Tracking
  tracking_device_activity activity                        () const
  {
    return static_cast<tracking_device_activity>(vr::VRSystem()->GetTrackedDeviceActivityLevel(index_));
  }
  hand                     hand                            () const
  {
    return static_cast<ne::hand>(vr::VRSystem()->GetControllerRoleForTrackedDeviceIndex(index_));
  }
                                                              
  // IVR System - Property                                    
  bool                     connected                       () const
  {
    return vr::VRSystem()->IsTrackedDeviceConnected(index_);
  }

  std::array<float, 12>    display_transform               () const
  {
    return get_property_matrix34(vr::Prop_StatusDisplayTransform_Matrix34);
  }
  
  std::string              resource_root                   () const
  {
    return get_property_string(vr::Prop_ResourceRoot_String);
  }
  std::string              render_model_name               () const
  {
    return get_property_string(vr::Prop_RenderModelName_String);
  }

  bool                     can_power_off                   () const
  {
    return get_property_bool(vr::Prop_DeviceCanPowerOff_Bool);
  }
  bool                     is_charging                     () const
  {
    return get_property_bool(vr::Prop_DeviceIsCharging_Bool);
  }
  bool                     has_battery_status              () const
  {
    return get_property_bool(vr::Prop_DeviceProvidesBatteryStatus_Bool);
  }
  float                    battery_percentage              () const
  {
    return get_property_float(vr::Prop_DeviceBatteryPercentage_Float);
  }
  
  bool                     is_wireless                     () const
  {
    return get_property_bool(vr::Prop_DeviceIsWireless_Bool);
  }
  std::uint64_t            dongle_version                  () const
  {
    return get_property_uint64(vr::Prop_DongleVersion_Uint64);
  }
  std::string              dongle_descriptions             () const
  {
    return get_property_string(vr::Prop_AllWirelessDongleDescriptions_String);
  }
  std::string              connected_dongle                () const
  {
    return get_property_string(vr::Prop_ConnectedWirelessDongle_String);
  }
  
  bool                     has_yaw_drift                   () const
  {
    return get_property_bool(vr::Prop_WillDriftInYaw_Bool);
  }
  bool                     has_proximity_sensor            () const
  {
    return get_property_bool(vr::Prop_ContainsProximitySensor_Bool);
  }
  bool                     has_camera                      () const
  {
    return get_property_bool(vr::Prop_HasCamera_Bool);
  }
  bool                     blocks_server_shutdown          () const
  {
    return get_property_bool(vr::Prop_BlockServerShutdown_Bool);
  }
  bool                     can_unify_coordinates_with_hmd  () const
  {
    return get_property_bool(vr::Prop_CanUnifyCoordinateSystemWithHmd_Bool);
  }
  
  std::string              manufacturer_name               () const
  {
    return get_property_string(vr::Prop_ManufacturerName_String);
  }
  std::string              tracking_system_name            () const
  {
    return get_property_string(vr::Prop_TrackingSystemName_String);
  }
  std::string              model_number                    () const
  {
    return get_property_string(vr::Prop_ModelNumber_String);
  }
  std::string              serial_number                   () const
  {
    return get_property_string(vr::Prop_SerialNumber_String);
  }
 
  std::uint64_t            hardware_revision               () const
  {
    return get_property_uint64(vr::Prop_HardwareRevision_Uint64);
  }
  std::string              hardware_revision_string        () const
  {
    return get_property_string(vr::Prop_HardwareRevision_String);
  }
  
  std::uint64_t            parent_driver                   () const
  {
    return get_property_uint64(vr::Prop_ParentDriver_Uint64);
  }
  std::string              driver_version                  () const
  {
    return get_property_string(vr::Prop_DriverVersion_String);
  }
  
  std::uint64_t            firmware_version                () const
  {
    return get_property_uint64(vr::Prop_FirmwareVersion_Uint64);
  }
  std::string              firmware_version_string         () const
  {
    return get_property_string(vr::Prop_TrackingFirmwareVersion_String);
  }
  std::string              firmware_programming_target     () const
  {
    return get_property_string(vr::Prop_Firmware_ProgrammingTarget_String);
  }
  bool                     firmware_update_available       () const
  {
    return get_property_bool(vr::Prop_Firmware_UpdateAvailable_Bool);
  }
  bool                     firmware_update_required        () const
  {
    return get_property_bool(vr::Prop_Firmware_ForceUpdateRequired_Bool);
  }
  bool                     firmware_manual_update          () const
  {
    return get_property_bool(vr::Prop_Firmware_ManualUpdate_Bool);
  }
  std::string              firmware_manual_update_url      () const
  {
    return get_property_string(vr::Prop_Firmware_ManualUpdateURL_String);
  }
  
  std::uint64_t            fpga_version                    () const
  {
    return get_property_uint64(vr::Prop_FPGAVersion_Uint64);
  }
  std::uint64_t            vrc_version                     () const
  {
    return get_property_uint64(vr::Prop_VRCVersion_Uint64);
  }
  std::uint64_t            radio_version                   () const
  {
    return get_property_uint64(vr::Prop_RadioVersion_Uint64);
  }

  std::string              registered_device_type          () const
  {
    return get_property_string(vr::Prop_RegisteredDeviceType_String);
  }
  std::string              input_profile_name              () const
  {
    return get_property_string(vr::Prop_InputProfileName_String);
  }
  
  // IVR System - Debug                                                                      
  std::string              send_driver_request             (const std::string& request) const
  {
    char raw_response[32768];
    vr::VRSystem()->DriverDebugRequest(index_, request.data(), &raw_response[0], 32768);
    return std::string(raw_response);
  }
                                                                                                        
  // IVR System - Firmware                                                                              
  void                     update_firmware                 () const
  {
    vr::VRSystem()->PerformFirmwareUpdate(index_);
  }
  
  std::uint32_t            index                           () const
  {
    return index_;
  }
  tracking_device_pose*    pose                            ()
  {
    return pose_.is_initialized() ? pose_.get_ptr() : nullptr;
  }

protected:                                                 
  // IVR System - Property                                 
  bool                     get_property_bool               (const vr::ETrackedDeviceProperty native_property) const
  {
    auto error = vr::TrackedProp_Success;
    return vr::VRSystem()->GetBoolTrackedDeviceProperty  (index_, native_property, &error);
  }
  float                    get_property_float              (const vr::ETrackedDeviceProperty native_property) const
  {
    auto error = vr::TrackedProp_Success;
    return vr::VRSystem()->GetFloatTrackedDeviceProperty (index_, native_property, &error);
  }
  int                      get_property_int                (const vr::ETrackedDeviceProperty native_property) const
  {
    auto error = vr::TrackedProp_Success;
    return vr::VRSystem()->GetInt32TrackedDeviceProperty (index_, native_property, &error);
  }
  std::uint64_t            get_property_uint64             (const vr::ETrackedDeviceProperty native_property) const
  {
    auto error = vr::TrackedProp_Success;
    return vr::VRSystem()->GetUint64TrackedDeviceProperty(index_, native_property, &error);
  }
  std::array<float, 12>    get_property_matrix34           (const vr::ETrackedDeviceProperty native_property) const
  {
    auto error = vr::TrackedProp_Success;
    const auto native_matrix = vr::VRSystem()->GetMatrix34TrackedDeviceProperty(index_, native_property, &error);
    std::array<float, 12> matrix; std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::string              get_property_string             (const vr::ETrackedDeviceProperty native_property) const
  {
    auto error = vr::TrackedProp_Success;
    char native_string[vr::k_unMaxPropertyStringSize];
    vr::VRSystem()->GetStringTrackedDeviceProperty(index_, native_property, native_string, vr::k_unMaxPropertyStringSize, &error);
    return std::string(native_string);
  }

  std::uint32_t                         index_;
  boost::optional<tracking_device_pose> pose_ ;
};

typedef tracking_device<tracking_device_type::generic> generic_tracking_device;
}

#endif