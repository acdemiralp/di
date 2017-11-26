#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_

#include <openvr.h>

#include <nano_engine/system.hpp>

namespace ne
{
class vr_system final : public system
{
public:
  vr_system           ()
  {
    auto error    = vr::VRInitError_None;
    vr_interface_ = vr::VR_Init(&error, vr::VRApplication_Scene);
    if  (error   != vr::VRInitError_None)
    {
      // Unable to find HMD. Proceed without.
    }
    auto chaperone     = static_cast<vr::IVRChaperone*>   (VR_GetGenericInterface(vr::IVRChaperone_Version   , &error));
    auto compositor    = static_cast<vr::IVRCompositor*>  (VR_GetGenericInterface(vr::IVRCompositor_Version  , &error));
    auto overlay       = static_cast<vr::IVROverlay*>     (VR_GetGenericInterface(vr::IVROverlay_Version     , &error));
    auto render_models = static_cast<vr::IVRRenderModels*>(VR_GetGenericInterface(vr::IVRRenderModels_Version, &error));
    auto screenshots   = static_cast<vr::IVRScreenshots*> (VR_GetGenericInterface(vr::IVRScreenshots_Version , &error));
  }
  vr_system           (const vr_system&  that) = default;
  vr_system           (      vr_system&& temp) = default;
  virtual ~vr_system  ()
  {
    vr::VR_Shutdown();
  }
  vr_system& operator=(const vr_system&  that) = default;
  vr_system& operator=(      vr_system&& temp) = default;

private:
  void tick() override
  {
    vr::VREvent_t event;
    while (vr_interface_->PollNextEvent(&event, sizeof event))
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

  vr::IVRSystem* vr_interface_;
};
}

#endif
