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
  
  static bool        available              ()
  {
    return hardware_present() && runtime_installed();
  }
  static bool        hardware_present       ()
  {
    return vr::VR_IsHmdPresent();
  }
  static bool        runtime_installed      ()
  {
    return vr::VR_IsRuntimeInstalled();
  }
  static std::string runtime_location       ()
  {
    return std::string(vr::VR_RuntimePath());
  }

  // IVR System - Controller
  void               set_input_focus        (const bool enabled)
  {
    enabled ? vr::VRSystem()->CaptureInputFocus() : vr::VRSystem()->ReleaseInputFocus();
  }
  
  // IVR System - Application Life Cycle
  void               acknowledge_exit       () const
  {
    vr::VRSystem()->AcknowledgeQuit_Exiting();
  }
  void               acknowledge_user_prompt() const
  {
    vr::VRSystem()->AcknowledgeQuit_UserPrompt();
  }
  
  std::vector<hmd>                                                    hmds               () const
  {

  }
  std::vector<vr_controller>                                          controllers        () const
  {

  }
  std::vector<tracking_reference>                                     tracking_references() const
  {
    
  }
  std::vector<display_redirect>                                       display_redirects  () const
  {

  }
  std::vector<tracking_device<tracking_device_type::generic_tracker>> generic_trackers   () const
  {

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
