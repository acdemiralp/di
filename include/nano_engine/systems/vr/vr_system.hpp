#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_

#include <stdexcept>
#include <string>
#include <vector>

#include <openvr.h>

#include <nano_engine/systems/vr/display_redirect.hpp>
#include <nano_engine/systems/vr/hmd.hpp>
#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/systems/vr/tracking_reference.hpp>
#include <nano_engine/systems/vr/vr_controller.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class vr_system final : public system
{
public:
  vr_system           ()
  {
    auto error = vr::VRInitError_None;
    VR_Init(&error, vr::VRApplication_Scene);
    if (error != vr::VRInitError_None)
      throw std::runtime_error("Failed to initialize OpenVR. Error " 
        + std::string(vr::VR_GetVRInitErrorAsSymbol            (error))
        + std::string(": ")
        + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(error)));
  }
  vr_system           (const vr_system&  that) = delete ;
  vr_system           (      vr_system&& temp) = default;
  virtual ~vr_system  ()
  {
    vr::VR_Shutdown();
  }
  vr_system& operator=(const vr_system&  that) = delete ;
  vr_system& operator=(      vr_system&& temp) = default;
  
  static bool                          available               ()
  {
    return hardware_present() && runtime_installed();
  }
  static bool                          hardware_present        ()
  {
    return vr::VR_IsHmdPresent();
  }
  static bool                          runtime_installed       ()
  {
    return vr::VR_IsRuntimeInstalled();
  }
  static std::string                   runtime_location        ()
  {
    return std::string(vr::VR_RuntimePath());
  }
  
  // IVR System - Tracking
  std::vector<hmd>                     hmds                    () const
  {
    std::vector<hmd> hmds;
    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(static_cast<vr::ETrackedDeviceClass>(tracking_device_type::hmd)               , indices.data(), indices.size());
    for (auto i = 0; i < count; i++) hmds.emplace_back(indices[i]);
    return hmds;
  }
  std::vector<vr_controller>           controllers             () const
  {
    std::vector<vr_controller> controllers;
    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(static_cast<vr::ETrackedDeviceClass>(tracking_device_type::controller)        , indices.data(), indices.size());
    for (auto i = 0; i < count; i++) controllers.emplace_back(indices[i]);
    return controllers;
  }
  std::vector<tracking_reference>      tracking_references     () const
  {
    std::vector<tracking_reference> tracking_references;
    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(static_cast<vr::ETrackedDeviceClass>(tracking_device_type::tracking_reference), indices.data(), indices.size());
    for (auto i = 0; i < count; i++) tracking_references.emplace_back(indices[i]);
    return tracking_references;
  }
  std::vector<display_redirect>        display_redirects       () const
  {
    std::vector<display_redirect> display_redirects;
    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(static_cast<vr::ETrackedDeviceClass>(tracking_device_type::display_redirect)  , indices.data(), indices.size());
    for (auto i = 0; i < count; i++) display_redirects.emplace_back(indices[i]);
    return display_redirects;
  }
  std::vector<generic_tracking_device> generic_tracking_devices() const
  {
    std::vector<generic_tracking_device> generic_tracking_devices;
    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(static_cast<vr::ETrackedDeviceClass>(tracking_device_type::generic)           , indices.data(), indices.size());
    for (auto i = 0; i < count; i++) generic_tracking_devices.emplace_back(indices[i]);
    return generic_tracking_devices;
  }
                                                      
  // IVR System - Controller                                   
  void                                 set_input_focus         (const bool enabled)
  {
    enabled ? vr::VRSystem()->CaptureInputFocus() : vr::VRSystem()->ReleaseInputFocus();
  }
                                                               
  // IVR System - Application Life Cycle                       
  void                                 acknowledge_exit        () const
  {
    vr::VRSystem()->AcknowledgeQuit_Exiting();
  }
  void                                 acknowledge_user_prompt () const
  {
    vr::VRSystem()->AcknowledgeQuit_UserPrompt();
  }
  
private:
  void tick() override
  {
    vr::VREvent_t event;
    while (vr::VRSystem()->PollNextEvent(&event, sizeof event))
    {
      if      (event.eventType == vr::VREvent_TrackedDeviceActivated  )
      {
        
      }
      else if (event.eventType == vr::VREvent_TrackedDeviceDeactivated)
      {
        
      }
      else if (event.eventType == vr::VREvent_TrackedDeviceUpdated    )
      {
        
      }
    }
  }
};
}

#endif
