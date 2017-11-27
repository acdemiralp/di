#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_

#include <array>
#include <cstddef>
#include <stdexcept>

#include <openvr.h>

#include <nano_engine/system.hpp>

namespace ne
{
class vr_system final : public system
{
public:
  vr_system           () 
  : native_       (VR_Init(&init_error_, vr::VRApplication_Scene))
  , chaperone_    (static_cast<vr::IVRChaperone*>   (VR_GetGenericInterface(vr::IVRChaperone_Version   , &interface_error_)))
  , compositor_   (static_cast<vr::IVRCompositor*>  (VR_GetGenericInterface(vr::IVRCompositor_Version  , &interface_error_)))
  , overlay_      (static_cast<vr::IVROverlay*>     (VR_GetGenericInterface(vr::IVROverlay_Version     , &interface_error_)))
  , render_models_(static_cast<vr::IVRRenderModels*>(VR_GetGenericInterface(vr::IVRRenderModels_Version, &interface_error_)))
  , screenshots_  (static_cast<vr::IVRScreenshots*> (VR_GetGenericInterface(vr::IVRScreenshots_Version , &interface_error_)))
  {
    if (init_error_      != vr::VRInitError_None)
      throw std::runtime_error("Failed to initialize OpenVR. Error: "          + std::string(VR_GetVRInitErrorAsSymbol(init_error_     )));
    if (interface_error_ != vr::VRInitError_None)
      throw std::runtime_error("Failed to retrieve OpenVR interfaces. Error: " + std::string(VR_GetVRInitErrorAsSymbol(interface_error_)));
  }
  vr_system           (const vr_system&  that) = delete ;
  vr_system           (      vr_system&& temp) = default;
  virtual ~vr_system  ()
  {
    vr::VR_Shutdown();
  }
  vr_system& operator=(const vr_system&  that) = delete ;
  vr_system& operator=(      vr_system&& temp) = default;

  std::array<std::size_t, 2> recommended_render_target_size() const
  {
    std::array<std::size_t, 2> size;
    native_->GetRecommendedRenderTargetSize(
      reinterpret_cast<std::uint32_t*>(&size[0]), 
      reinterpret_cast<std::uint32_t*>(&size[1]));
    return size;
  }

  static bool                hardware_present              ()
  {
    return vr::VR_IsHmdPresent();
  }
  static bool                runtime_installed             ()
  {
    return vr::VR_IsRuntimeInstalled();
  }
  static std::string         runtime_location              ()
  {
    return std::string(vr::VR_RuntimePath());
  }

private:
  void tick() override
  {
    vr::VREvent_t event;
    while (native_->PollNextEvent(&event, sizeof event))
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

  vr::EVRInitError     init_error_      = vr::VRInitError_None;
  vr::EVRInitError     interface_error_ = vr::VRInitError_None;
  vr::IVRSystem*       native_          = nullptr;
  vr::IVRChaperone*    chaperone_       = nullptr;
  vr::IVRCompositor*   compositor_      = nullptr;
  vr::IVROverlay*      overlay_         = nullptr;
  vr::IVRRenderModels* render_models_   = nullptr;
  vr::IVRScreenshots*  screenshots_     = nullptr;
};
}

#endif
