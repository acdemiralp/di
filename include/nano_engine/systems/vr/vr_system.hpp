#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <openvr.h>

#include <nano_engine/systems/vr/compositor.hpp>
#include <nano_engine/systems/vr/display_redirect.hpp>
#include <nano_engine/systems/vr/hmd.hpp>
#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/systems/vr/tracking_mode.hpp>
#include <nano_engine/systems/vr/tracking_reference.hpp>
#include <nano_engine/systems/vr/vr_controller.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class vr_system final : public system
{
public:
  explicit vr_system  (const tracking_mode tracking_mode) : tracking_mode_(tracking_mode)
  {
    auto error = vr::VRInitError_None;
    VR_Init(&error, vr::VRApplication_Scene);
    if (error != vr::VRInitError_None)
      throw std::runtime_error("Failed to initialize OpenVR. Error " 
        + std::string(vr::VR_GetVRInitErrorAsSymbol            (error))
        + std::string(": ")
        + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(error)));

    compositor_ = std::make_unique<ne::compositor>(tracking_mode_);
    if (!compositor_)
      throw std::runtime_error("Failed to retrieve the OpenVR compositor.");

    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count  = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_HMD        , indices.data(), static_cast<std::uint32_t>(indices.size())); 
    for (auto i = 0u; i < count; i++) 
      hmds_                    .push_back(std::make_unique<hmd>                       (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_Controller       , indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; i++) 
      controllers_             .emplace_back(std::make_unique<vr_controller>          (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; i++) 
      tracking_references_     .emplace_back(std::make_unique<tracking_reference>     (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_DisplayRedirect  , indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; i++) 
      display_redirects_       .emplace_back(std::make_unique<display_redirect>       (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_GenericTracker   , indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; i++) 
      generic_tracking_devices_.emplace_back(std::make_unique<generic_tracking_device>(indices[i]));
  }
  vr_system           (const vr_system&  that) = delete ;
  vr_system           (      vr_system&& temp) = default;
  virtual ~vr_system  ()
  {
    vr::VR_Shutdown();
  }
  vr_system& operator=(const vr_system&  that) = delete ;
  vr_system& operator=(      vr_system&& temp) = default;
  
  static bool                          available                ()
  {
    return hardware_present() && runtime_installed();
  }
  static bool                          hardware_present         ()
  {
    return vr::VR_IsHmdPresent();
  }
  static bool                          runtime_installed        ()
  {
    return vr::VR_IsRuntimeInstalled();
  }
  static std::string                   runtime_location         ()
  {
    return std::string(vr::VR_RuntimePath());
  }
  
  // IVR System - Tracking
  std::vector<hmd*>                     hmds                    () const
  {
    std::vector<hmd*> hmds(hmds_.size());
    std::transform(
      hmds_.begin(),
      hmds_.end  (),
      hmds .begin(),
      [ ] (const std::unique_ptr<hmd>& hmd)
      {
        return hmd.get();
      });
    return hmds;
  }
  std::vector<vr_controller*>           controllers             () const
  {
    std::vector<vr_controller*> controllers(controllers_.size());
    std::transform(
      controllers_.begin(),
      controllers_.end  (),
      controllers .begin(),
      [ ] (const std::unique_ptr<vr_controller>& controller)
      {
        return controller.get();
      });
    return controllers;
  }
  std::vector<tracking_reference*>      tracking_references     () const
  {
    std::vector<tracking_reference*> tracking_references(tracking_references_.size());
    std::transform(
      tracking_references_.begin(),
      tracking_references_.end  (),
      tracking_references .begin(),
      [ ] (const std::unique_ptr<tracking_reference>& tracking_reference)
      {
        return tracking_reference.get();
      });
    return tracking_references;
  }
  std::vector<display_redirect*>        display_redirects       () const
  {
    std::vector<display_redirect*> display_redirects(display_redirects_.size());
    std::transform(
      display_redirects_.begin(),
      display_redirects_.end  (),
      display_redirects .begin(),
      [ ] (const std::unique_ptr<display_redirect>& display_redirect)
      {
        return display_redirect.get();
      });
    return display_redirects;
  }
  std::vector<generic_tracking_device*> generic_tracking_devices() const
  {
    std::vector<generic_tracking_device*> generic_tracking_devices(generic_tracking_devices_.size());
    std::transform(
      generic_tracking_devices_.begin(),
      generic_tracking_devices_.end  (),
      generic_tracking_devices .begin(),
      [ ] (const std::unique_ptr<generic_tracking_device>& generic_tracking_device)
      {
        return generic_tracking_device.get();
      });
    return generic_tracking_devices;
  }

  // IVR System - Controller                                   
  void                                 set_input_focus          (const bool enabled)
  {
    enabled ? vr::VRSystem()->CaptureInputFocus() : vr::VRSystem()->ReleaseInputFocus();
  }
                                                               
  // IVR System - Application Life Cycle                       
  void                                 acknowledge_exit         () const
  {
    vr::VRSystem()->AcknowledgeQuit_Exiting();
  }
  void                                 acknowledge_user_prompt  () const
  {
    vr::VRSystem()->AcknowledgeQuit_UserPrompt();
  }
                                                               
  void                                 set_tracking_mode        (const tracking_mode tracking_mode)
  {
    tracking_mode_ = tracking_mode;
  }
  compositor*                          compositor               () const
  {
    return compositor_.get();
  }

private:
  void                                 pre_tick                 () override
  {
    // Shallow pass: Low accuracy pose predictions of the tracking devices.
    vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(static_cast<vr::ETrackingUniverseOrigin>(tracking_mode_), hmds_.size() > 0 ? hmds()[0]->seconds_to_photons() : 0.016F, poses, vr::k_unMaxTrackedDeviceCount);
    for (auto& hmd                     : hmds_                    ) hmd                    ->pose_ = tracking_device_pose(poses[hmd                    ->index()]);
    for (auto& controller              : controllers_             ) controller             ->pose_ = tracking_device_pose(poses[controller             ->index()]);
    for (auto& tracking_reference      : tracking_references_     ) tracking_reference     ->pose_ = tracking_device_pose(poses[tracking_reference     ->index()]);
    for (auto& display_redirect        : display_redirects_       ) display_redirect       ->pose_ = tracking_device_pose(poses[display_redirect       ->index()]);
    for (auto& generic_tracking_device : generic_tracking_devices_) generic_tracking_device->pose_ = tracking_device_pose(poses[generic_tracking_device->index()]);
  }
  void                                 tick                     () override
  {
    // Deep pass: High accuracy poses of the tracking devices. Freezes the calling thread. Use just prior to rendering in order to correct the eye transforms.
    vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
    vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
    for (auto& hmd                     : hmds_                    ) hmd                    ->pose_ = tracking_device_pose(poses[hmd                    ->index()]);
    for (auto& controller              : controllers_             ) controller             ->pose_ = tracking_device_pose(poses[controller             ->index()]);
    for (auto& tracking_reference      : tracking_references_     ) tracking_reference     ->pose_ = tracking_device_pose(poses[tracking_reference     ->index()]);
    for (auto& display_redirect        : display_redirects_       ) display_redirect       ->pose_ = tracking_device_pose(poses[display_redirect       ->index()]);
    for (auto& generic_tracking_device : generic_tracking_devices_) generic_tracking_device->pose_ = tracking_device_pose(poses[generic_tracking_device->index()]);

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

  tracking_mode                                         tracking_mode_           ;
  std::unique_ptr<ne::compositor>                       compositor_              ;
  std::vector<std::unique_ptr<hmd>>                     hmds_                    ;
  std::vector<std::unique_ptr<vr_controller>>           controllers_             ;
  std::vector<std::unique_ptr<tracking_reference>>      tracking_references_     ;
  std::vector<std::unique_ptr<display_redirect>>        display_redirects_       ;
  std::vector<std::unique_ptr<generic_tracking_device>> generic_tracking_devices_;
};
}

#endif
