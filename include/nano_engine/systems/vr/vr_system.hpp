#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_

#include <algorithm>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/signals2.hpp>
#include <openvr.h>

#include <nano_engine/systems/vr/chaperone.hpp>
#include <nano_engine/systems/vr/dashboard_overlay.hpp>
#include <nano_engine/systems/vr/display_redirect.hpp>
#include <nano_engine/systems/vr/hmd.hpp>
#include <nano_engine/systems/vr/overlay.hpp>
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
  explicit vr_system  (const tracking_mode tracking_mode)
  {
    auto error = vr::VRInitError_None;
    VR_Init(&error, vr::VRApplication_Scene);
    if (error != vr::VRInitError_None)
      throw std::runtime_error("Failed to initialize OpenVR. Error " 
        + std::string(vr::VR_GetVRInitErrorAsSymbol            (error))
        + std::string(": ")
        + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(error)));

    chaperone_ = std::make_unique<ne::chaperone>();

    std::vector<std::uint32_t> indices(vr::k_unMaxTrackedDeviceCount);
    auto count  = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_HMD        , indices.data(), static_cast<std::uint32_t>(indices.size())); 
    for (auto i = 0u; i < count; ++i) 
      hmds_                    .emplace_back(std::make_unique<hmd>                    (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_Controller       , indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; ++i) 
      controllers_             .emplace_back(std::make_unique<vr_controller>          (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; ++i) 
      tracking_references_     .emplace_back(std::make_unique<tracking_reference>     (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_DisplayRedirect  , indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; ++i) 
      display_redirects_       .emplace_back(std::make_unique<display_redirect>       (indices[i]));
    count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_GenericTracker   , indices.data(), static_cast<std::uint32_t>(indices.size()));
    for (auto i = 0u; i < count; ++i) 
      generic_tracking_devices_.emplace_back(std::make_unique<generic_tracking_device>(indices[i]));
    
    set_tracking_mode(tracking_mode);
  }
  vr_system           (const vr_system&  that) = delete ;
  vr_system           (      vr_system&& temp) = delete ;
  ~vr_system          ()
  {
    vr::VR_Shutdown();
  }
  vr_system& operator=(const vr_system&  that) = delete ;
  vr_system& operator=(      vr_system&& temp) = delete ;
  
  static bool                           available                ()
  {
    return hardware_present() && runtime_installed();
  }
  static bool                           hardware_present         ()
  {
    return vr::VR_IsHmdPresent();
  }
  static bool                           runtime_installed        ()
  {
    return vr::VR_IsRuntimeInstalled();
  }
  static std::string                    runtime_location         ()
  {
    return std::string(vr::VR_RuntimePath());
  }
  
  // IVR System - Tracking
  std::vector<hmd*>                     hmds                     () const
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
  std::vector<vr_controller*>           controllers              () const
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
  std::vector<tracking_reference*>      tracking_references      () const
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
  std::vector<display_redirect*>        display_redirects        () const
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
  std::vector<generic_tracking_device*> generic_tracking_devices () const
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
  void                                  set_input_focus          (const bool enabled)
  {
    enabled ? vr::VRSystem()->CaptureInputFocus() : vr::VRSystem()->ReleaseInputFocus();
  }
                                                                 
  // IVR System - Application Life Cycle                         
  void                                  acknowledge_exit         () const
  {
    vr::VRSystem()->AcknowledgeQuit_Exiting();
  }
  void                                  acknowledge_user_prompt  () const
  {
    vr::VRSystem()->AcknowledgeQuit_UserPrompt();
  }
  
  // IVR Compositor
  void                                  set_tracking_mode        (const tracking_mode tracking_mode)
  {
    if (tracking_mode == tracking_mode::automatic) return;
    vr::VRCompositor()->SetTrackingSpace(static_cast<vr::ETrackingUniverseOrigin>(tracking_mode));
  }                                                           
  tracking_mode                         tracking_mode            () const
  {
    return static_cast<ne::tracking_mode>(vr::VRCompositor()->GetTrackingSpace());
  }

  // IVR Overlay
  template<typename... argument_types>
  overlay*                              create_overlay           (argument_types&&... arguments)
  {
    overlays_.emplace_back(std::make_unique<overlay>(arguments...));
    return static_cast<overlay*>(overlays_.back().get());
  }
  template<typename... argument_types>
  dashboard_overlay*                    create_dashboard_overlay (argument_types&&... arguments)
  {
    overlays_.emplace_back(std::make_unique<dashboard_overlay>(arguments...));
    return static_cast<dashboard_overlay*>(overlays_.back().get());
  }
  void                                  destroy_overlay          (overlay* overlay)
  {
    overlays_.erase(std::remove_if(
      overlays_.begin(), 
      overlays_.end  (), 
      [&overlay] (const std::unique_ptr<ne::overlay>& iteratee)
      {
        return iteratee.get() == overlay;
      }), 
      overlays_.end  ());
  }
  std::vector<overlay*>                 overlays                 () const
  {
    std::vector<overlay*> overlays(overlays_.size());
    std::transform(
      overlays_.begin(), 
      overlays_.end  (), 
      overlays .begin(), 
      [ ] (const std::unique_ptr<overlay>& overlay)
      {
        return overlay.get();
      });
    return overlays;
  }

  // IVR Resources
  std::string                           load_resource            (const std::string& name)
  {
    std::string resource(vr::VRResources()->LoadSharedResource(name.c_str(), nullptr, 0), ' ');
    vr::VRResources()->LoadSharedResource(name.c_str(), &resource[0], static_cast<std::uint32_t>(resource.size()));
    return resource;
  }
  std::string                           resource_full_path       (const std::string& name, const std::string& directory = std::string()) const
  {
    char path[vr::k_unMaxPropertyStringSize];
    vr::VRResources()->GetResourceFullPath(name.c_str(), directory.c_str(), path, vr::k_unMaxPropertyStringSize);
    return std::string(path);
  }

  // IVR Driver Manager
  std::vector<std::string>              drivers                  () const
  {
    std::vector<std::string> drivers(vr::VRDriverManager()->GetDriverCount());
    for (auto i = 0; i < drivers.size(); ++i)
    {
      char driver[vr::k_unMaxPropertyStringSize];
      vr::VRDriverManager()->GetDriverName(static_cast<vr::DriverId_t>(i), driver, vr::k_unMaxPropertyStringSize);
      drivers[i] = driver;
    }
    return drivers;
  }

  // Auxiliary
  chaperone*                            chaperone                ()
  {
    return chaperone_.get();
  }
  
  boost::signals2::signal<void(                                                                )> device_event                 ;
  boost::signals2::signal<void(vr_controller_button                                            )> button_event                 ;
  boost::signals2::signal<void(std::array<float, 2>                                            )> mouse_move_event             ;
  boost::signals2::signal<void(std::size_t                                                     )> mouse_press_event            ;
  boost::signals2::signal<void(std::array<float, 2>, std::size_t                               )> scroll_event                 ;
  boost::signals2::signal<void(/* TODO */                                                      )> touchpad_move_event          ;
  boost::signals2::signal<void(std::uint64_t, std::uint32_t                                    )> notification_event           ;
  boost::signals2::signal<void(std::uint32_t, std::uint32_t, bool                              )> process_event                ;
  boost::signals2::signal<void(std::uint64_t                                                   )> overlay_event                ;
  boost::signals2::signal<void(vr::EVRState                                                    )> state_event                  ;
  boost::signals2::signal<void(std::string, std::uint64_t                                      )> keyboard_event               ;
  boost::signals2::signal<void(float                                                           )> interpupillary_distance_event;
  boost::signals2::signal<void(std::uint64_t, std::uint64_t                                    )> chaperone_event              ;
  boost::signals2::signal<void(std::uint32_t                                                   )> performance_test_event       ;
  boost::signals2::signal<void(bool                                                            )> seated_pose_reset_event      ;
  boost::signals2::signal<void(std::uint32_t, vr_screenshot_type                               )> screenshot_event             ;
  boost::signals2::signal<void(float                                                           )> screenshot_progress_event    ;
  boost::signals2::signal<void(std::uint32_t, std::uint32_t                                    )> application_launch_event     ;
  boost::signals2::signal<void(std::uint64_t, std::uint32_t                                    )> camera_surface_edit_event    ;
  boost::signals2::signal<void(std::uint32_t                                                   )> message_overlay_event        ;
  boost::signals2::signal<void(vr::PropertyContainerHandle_t, vr::ETrackedDeviceProperty       )> property_event               ;
  boost::signals2::signal<void(std::array<float, 2>, std::array<float, 2>, vr::EDualAnalogWhich)> dual_analog_event            ;

private:                                                        
  void                                  pre_tick                 () override
  {
    // Shallow pass: Low accuracy pose predictions of the tracking devices.
    vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(static_cast<vr::ETrackingUniverseOrigin>(tracking_mode()), hmds_.size() > 0 ? hmds()[0]->time_to_photons().count() : 0.016F, poses, vr::k_unMaxTrackedDeviceCount);
    for (auto& hmd                     : hmds_                    ) hmd                    ->pose_ = tracking_device_pose(poses[hmd                    ->index()]);
    for (auto& controller              : controllers_             ) controller             ->pose_ = tracking_device_pose(poses[controller             ->index()]);
    for (auto& tracking_reference      : tracking_references_     ) tracking_reference     ->pose_ = tracking_device_pose(poses[tracking_reference     ->index()]);
    for (auto& display_redirect        : display_redirects_       ) display_redirect       ->pose_ = tracking_device_pose(poses[display_redirect       ->index()]);
    for (auto& generic_tracking_device : generic_tracking_devices_) generic_tracking_device->pose_ = tracking_device_pose(poses[generic_tracking_device->index()]);
  }
  void                                  tick                     () override
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
    while (vr::VRSystem()->PollNextEvent(&event, sizeof vr::VREvent_t))
    {
      // TODO: Process events.
    }

    for (auto& overlay : overlays_)
      overlay->process_events();
  }

  std::unique_ptr<ne::chaperone>                        chaperone_               ;
  
  std::vector<std::unique_ptr<hmd>>                     hmds_                    ;
  std::vector<std::unique_ptr<vr_controller>>           controllers_             ;
  std::vector<std::unique_ptr<tracking_reference>>      tracking_references_     ;
  std::vector<std::unique_ptr<display_redirect>>        display_redirects_       ;
  std::vector<std::unique_ptr<generic_tracking_device>> generic_tracking_devices_;

  std::vector<std::unique_ptr<overlay>>                 overlays_                ;
};
}

#endif
