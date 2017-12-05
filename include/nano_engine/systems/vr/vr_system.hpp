#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SYSTEM_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

#include <openvr.h>

#include <nano_engine/systems/vr/eye.hpp>
#include <nano_engine/system.hpp>

#undef near
#undef far

namespace ne
{
class vr_system final : public system
{
public:
  vr_system           () 
  : native_          (VR_Init(&init_error_, vr::VRApplication_Scene))
  , chaperone_       (vr::VRChaperone      ())
  , chaperone_setup_ (vr::VRChaperoneSetup ())
  , compositor_      (vr::VRCompositor     ())
  , overlay_         (vr::VROverlay        ())
  , screenshots_     (vr::VRScreenshots    ())
  , render_models_   (vr::VRRenderModels   ())
  , applications_    (vr::VRApplications   ())
  , settings_        (vr::VRSettings       ())
  , resources_       (vr::VRResources      ())
  , extended_display_(vr::VRExtendedDisplay())
  , tracked_camera_  (vr::VRTrackedCamera  ())
  , driver_manager_  (vr::VRDriverManager  ())

  {
    if (init_error_ != vr::VRInitError_None)
      throw std::runtime_error("Failed to initialize OpenVR. Error " 
        + std::string(vr::VR_GetVRInitErrorAsSymbol            (init_error_))
        + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(init_error_)));
  }
  vr_system           (const vr_system&  that) = delete ;
  vr_system           (      vr_system&& temp) = default;
  virtual ~vr_system  ()
  {
    vr::VR_Shutdown();
  }
  vr_system& operator=(const vr_system&  that) = delete ;
  vr_system& operator=(      vr_system&& temp) = default;

  // IVR System - Display
  std::array<std::size_t, 2>          recommended_render_target_size()                                        const
  {
    std::array<std::size_t, 2> size;
    native_->GetRecommendedRenderTargetSize(
      reinterpret_cast<std::uint32_t*>(&size[0]), 
      reinterpret_cast<std::uint32_t*>(&size[1]));
    return size;
  }
  std::array<float, 16>               projection_matrix             (eye eye, float near, float far)          const
  {
    std::array<float, 16> matrix;
    auto native_matrix = native_->GetProjectionMatrix(static_cast<vr::EVREye>(eye), near, far);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 16, matrix.data());
    return matrix;
  }
  std::array<float, 4>                projection_parameters         (eye eye)                                 const
  {
    std::array<float, 4> parameters;
    native_->GetProjectionRaw(static_cast<vr::EVREye>(eye), &parameters[0], &parameters[1], &parameters[2], &parameters[3]);
    return parameters;
  }
  std::array<std::array<float, 2>, 3> compute_distortion            (eye eye, const std::array<float, 2>& uv) const
  {
    std::array<std::array<float, 2>, 3> distortion;
    vr::DistortionCoordinates_t native_distortion;
    native_->ComputeDistortion(static_cast<vr::EVREye>(eye), uv[0], uv[1], &native_distortion);
    distortion[0][0] = native_distortion.rfRed  [0]; distortion[0][1] = native_distortion.rfRed  [1];
    distortion[1][0] = native_distortion.rfGreen[0]; distortion[1][1] = native_distortion.rfGreen[1];
    distortion[2][0] = native_distortion.rfBlue [0]; distortion[2][1] = native_distortion.rfBlue [1];
    return distortion;
  }
  std::array<float, 12>               eye_to_head_transform         (eye eye)                                 const
  {
    std::array<float, 12> matrix;
    auto native_matrix = native_->GetEyeToHeadTransform(static_cast<vr::EVREye>(eye));
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.data());
    return matrix;
  }

  float                               time_since_last_vsync         ()                                        const
  {
    float         time       ;
    std::uint64_t frame_count;
    native_->GetTimeSinceLastVsync(&time, &frame_count);
    return time;
  }
  std::uint64_t                       frame_count                   ()                                        const
  {
    float         time       ;
    std::uint64_t frame_count;
    native_->GetTimeSinceLastVsync(&time, &frame_count);
    return frame_count;
  }

  std::int32_t                        d3d9_output_info              ()                                        const
  {
    return native_->GetD3D9AdapterIndex();
  }
  std::int32_t                        d3d11_output_info             ()                                        const
  {
    std::int32_t index;
    native_->GetDXGIOutputInfo(&index);
    return index;
  }


  // IVR System - Display Mode

  // IVR System - Tracking

  static bool                         available                     ()
  {
    return hardware_present() && runtime_installed();
  }
  static bool                         hardware_present              ()
  {
    return vr::VR_IsHmdPresent();
  }
  static bool                         runtime_installed             ()
  {
    return vr::VR_IsRuntimeInstalled();
  }
  static std::string                  runtime_location              ()
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

  vr::EVRInitError        init_error_       = vr::VRInitError_None;
  vr::IVRSystem*          native_           = nullptr;
  vr::IVRChaperone*       chaperone_        = nullptr;
  vr::IVRChaperoneSetup*  chaperone_setup_  = nullptr;
  vr::IVRCompositor*      compositor_       = nullptr;
  vr::IVROverlay*         overlay_          = nullptr;
  vr::IVRScreenshots*     screenshots_      = nullptr;
  vr::IVRRenderModels*    render_models_    = nullptr;
  vr::IVRApplications*    applications_     = nullptr;
  vr::IVRSettings*        settings_         = nullptr;
  vr::IVRResources*       resources_        = nullptr;
  vr::IVRExtendedDisplay* extended_display_ = nullptr;
  vr::IVRTrackedCamera*   tracked_camera_   = nullptr;
  vr::IVRDriverManager*   driver_manager_   = nullptr;
};
}

#endif
