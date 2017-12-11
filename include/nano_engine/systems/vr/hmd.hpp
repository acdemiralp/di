#ifndef NANO_ENGINE_SYSTEMS_VR_HMD_HPP_
#define NANO_ENGINE_SYSTEMS_VR_HMD_HPP_

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <openvr.h>

#include <nano_engine/systems/vr/eye.hpp>
#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/systems/vr/tracking_device_pose.hpp>
#include <nano_engine/systems/vr/tracking_mode.hpp>

#ifdef _WIN32
#include <winnt.h>
#endif

#undef near
#undef far

typedef struct VkInstance_T*       VkInstance      ;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;

namespace ne
{
class hmd : public tracking_device<tracking_device_type::hmd>
{
public:
  explicit hmd  (const std::uint32_t& index) : tracking_device<tracking_device_type::hmd>(index)
  {
  
  }
  hmd           (const hmd&  that) = default;
  hmd           (      hmd&& temp) = default;
  virtual ~hmd  ()                 = default;
  hmd& operator=(const hmd&  that) = default;
  hmd& operator=(      hmd&& temp) = default;
  
  // IVR System - Display
  std::array<std::size_t, 2>          recommended_render_target_size      ()                                             const
  {
    std::array<std::size_t, 2> size;
    vr::VRSystem()->GetRecommendedRenderTargetSize(
      reinterpret_cast<std::uint32_t*>(&size[0]), 
      reinterpret_cast<std::uint32_t*>(&size[1]));
    return size;
  }
  std::array<float, 16>               projection_matrix                   (eye eye, const float near, const float far)   const
  {
    std::array<float, 16> matrix;
    const auto native_matrix = vr::VRSystem()->GetProjectionMatrix(static_cast<vr::EVREye>(eye), near, far);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 16, matrix.begin());
    return matrix;
  }
  std::array<float, 4>                projection_parameters               (eye eye)                                      const
  {
    std::array<float, 4> parameters;
    vr::VRSystem()->GetProjectionRaw(static_cast<vr::EVREye>(eye), &parameters[0], &parameters[1], &parameters[2], &parameters[3]);
    return parameters;
  }
  std::array<float, 12>               eye_to_head_transform               (eye eye)                                      const
  {
    std::array<float, 12> matrix;
    const auto native_matrix = vr::VRSystem()->GetEyeToHeadTransform(static_cast<vr::EVREye>(eye));
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::array<std::array<float, 2>, 3> compute_distortion                  (eye eye, const std::array<float, 2>& uv)      const
  {
    std::array<std::array<float, 2>, 3> distortion;
    vr::DistortionCoordinates_t native_distortion;
    vr::VRSystem()->ComputeDistortion(static_cast<vr::EVREye>(eye), uv[0], uv[1], &native_distortion);
    distortion[0][0] = native_distortion.rfRed  [0]; distortion[0][1] = native_distortion.rfRed  [1];
    distortion[1][0] = native_distortion.rfGreen[0]; distortion[1][1] = native_distortion.rfGreen[1];
    distortion[2][0] = native_distortion.rfBlue [0]; distortion[2][1] = native_distortion.rfBlue [1];
    return distortion;
  }
                                                                                                                         
  float                               time_since_last_vsync               ()                                             const
  {
    float         time       ;
    std::uint64_t frame_count;
    vr::VRSystem()->GetTimeSinceLastVsync(&time, &frame_count);
    return time;
  }
  std::uint64_t                       frame_count                         ()                                             const
  {
    float         time       ;
    std::uint64_t frame_count;
    vr::VRSystem()->GetTimeSinceLastVsync(&time, &frame_count);
    return frame_count;
  }
                                                                                                                         
#ifdef _WIN32                                                                                                            
  std::uint32_t                       adapter_index_d3d9                  ()                                             const
  {
    return static_cast<std::uint32_t>(vr::VRSystem()->GetD3D9AdapterIndex());
  }
  std::uint32_t                       adapter_index_d3d10_d3d11           ()                                             const
  {
    std::int32_t index;
    vr::VRSystem()->GetDXGIOutputInfo(&index);
    return static_cast<std::uint32_t>(index);
  }
  LUID                                output_device_d3d10_d3d11           ()                                             const
  {
    LUID   luid;
    vr::VRSystem()->GetOutputDevice(reinterpret_cast<std::uint64_t*>(&luid), vr::ETextureType::TextureType_DirectX);
    return luid;
  }
  LUID                                output_device_d3d12                 ()                                             const
  {
    LUID   luid;
    vr::VRSystem()->GetOutputDevice(reinterpret_cast<std::uint64_t*>(&luid), vr::ETextureType::TextureType_DirectX12);
    return luid;
  }                                                             
#elif __APPLE__                                                        
  std::uint64_t                       output_device_metal                 ()                                             const
  {
    std::uint64_t id;
    vr::VRSystem()->GetOutputDevice(&id, vr::ETextureType::TextureType_IOSurface);
    return        id;
  }                                                                                                                                                             
#endif                                                                                                                   
  VkPhysicalDevice                    output_device_vulkan                (VkInstance instance)                          const
  {
    VkPhysicalDevice device;
    vr::VRSystem()->GetOutputDevice(reinterpret_cast<std::uint64_t*>(&device), vr::ETextureType::TextureType_Vulkan, instance);
    return           device;
  }
                                                                                                                         
  // IVR System - Display Mode                                                                                           
  bool                                extended_display_mode               ()                                             const
  {
    return vr::VRSystem()->IsDisplayOnDesktop();
  }
  void                                set_extended_display_mode           (const bool enabled)
  {
    vr::VRSystem()->SetDisplayVisibility(enabled);
  }
                                                                          
  // IVR System - Tracking
  void                                set_current_pose_as_seated_zero_pose()
  {
    vr::VRSystem()->ResetSeatedZeroPose();
  }
  std::array<float, 12>               seated_zero_pose_to_standing_pose   ()                                             const
  {
    std::array<float, 12> matrix;
    const auto native_matrix = vr::VRSystem()->GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::array<float, 12>               raw_zero_pose_to_standing_pose      ()                                             const
  {
    std::array<float, 12> matrix;
    const auto native_matrix = vr::VRSystem()->GetRawZeroPoseToStandingAbsoluteTrackingPose();
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  
  // IVR System - Property
  std::array<float, 12>               camera_to_head_transform            ()                                             const
  {
    return get_property_matrix34(vr::Prop_CameraToHeadTransform_Matrix34);
  } 
  std::array<float, 2>                lens_center_uv                      (eye eye)                                      const
  {
    return eye == eye::left ? 
      std::array<float, 2>{get_property_float(vr::Prop_LensCenterLeftU_Float ), get_property_float(vr::Prop_LensCenterLeftV_Float )} :
      std::array<float, 2>{get_property_float(vr::Prop_LensCenterRightU_Float), get_property_float(vr::Prop_LensCenterRightV_Float)} ;
  }
  std::array<float, 2>                screenshot_field_of_view            ()                                             const
  {
    return 
    {
      get_property_float(vr::Prop_ScreenshotHorizontalFieldOfViewDegrees_Float),
      get_property_float(vr::Prop_ScreenshotVerticalFieldOfViewDegrees_Float  )
    };
  }
  
  float                               head_to_eye_depth_distance          ()                                             const
  {
    return get_property_float(vr::Prop_UserHeadToEyeDepthMeters_Float);
  }
  float                               interpupillary_distance             ()                                             const
  {
    return get_property_float(vr::Prop_UserIpdMeters_Float);
  }

  float                               seconds_from_vsync_to_photons       ()                                             const
  {
    return get_property_float(vr::Prop_SecondsFromVsyncToPhotons_Float);
  }
  float                               seconds_from_photons_to_vblank      ()                                             const
  {
    return get_property_float(vr::Prop_SecondsFromPhotonsToVblank_Float);
  }
  float                               display_frequency                   ()                                             const
  {
    return get_property_float(vr::Prop_DisplayFrequency_Float);
  }
  
  bool                                direct_mode_sends_vsync_events      ()                                             const
  {
    return get_property_bool(vr::Prop_DriverDirectModeSendsVsyncEvents_Bool);
  }
  bool                                reports_time_since_vsync            ()                                             const
  {
    return get_property_bool(vr::Prop_ReportsTimeSinceVSync_Bool);
  }
  bool                                on_desktop                          ()                                             const
  {
    return get_property_bool(vr::Prop_IsOnDesktop_Bool);
  }
  bool                                prediction                          ()                                             const
  {
    return !get_property_bool(vr::Prop_DoNotApplyPrediction_Bool);
  }
  bool                                suppressed                          ()                                             const
  {
    return get_property_bool(vr::Prop_DisplaySuppressed_Bool);
  }
  bool                                night_mode_support                  ()                                             const
  {
    return get_property_bool(vr::Prop_DisplayAllowNightMode_Bool);
  }
  bool                                debug                               ()                                             const
  {
    return get_property_bool(vr::Prop_DisplayDebugMode_Bool);
  }
  
  std::size_t                         expected_controller_count           ()                                             const
  {
    return static_cast<std::size_t>(get_property_int(vr::Prop_ExpectedControllerCount_Int32));
  }
  std::size_t                         expected_tracking_reference_count   ()                                             const
  {
    return static_cast<std::size_t>(get_property_int(vr::Prop_ExpectedTrackingReferenceCount_Int32));
  }

  std::uint64_t                       current_universe_id                 ()                                             const
  {
    return get_property_uint64(vr::Prop_CurrentUniverseId_Uint64);
  }
  std::uint64_t                       previous_universe_id                ()                                             const
  {
    return get_property_uint64(vr::Prop_PreviousUniverseId_Uint64);
  }
  
  int                                 mc_type                             ()                                             const
  {
    return get_property_int(vr::Prop_DisplayMCType_Int32);
  }
  float                               mc_offset                           ()                                             const
  {
    return get_property_float(vr::Prop_DisplayMCOffset_Float);
  }
  float                               mc_scale                            ()                                             const
  {
    return get_property_float(vr::Prop_DisplayMCScale_Float);
  }
  std::array<std::size_t, 2>          mc_image_size                       ()                                             const
  {
    return 
    {
      static_cast<std::size_t>(get_property_int(vr::Prop_DisplayMCImageWidth_Int32 )), 
      static_cast<std::size_t>(get_property_int(vr::Prop_DisplayMCImageHeight_Int32))
    };
  }
  std::size_t                         mc_image_num_channels               ()                                             const
  {
    return static_cast<std::size_t>(get_property_int(vr::Prop_DisplayMCImageNumChannels_Int32));
  }
  std::string                         mc_image_path                       (eye eye)                                      const
  {
    return get_property_string(eye == eye::left ? vr::Prop_DisplayMCImageLeft_String : vr::Prop_DisplayMCImageRight_String);
  }
  void*                               mc_image                            ()                                             const
  {
    return reinterpret_cast<void*>(get_property_uint64(vr::Prop_DisplayMCImageData_Binary));
  }

  int                                 gc_type                             ()                                             const
  {
    return get_property_int(vr::Prop_DisplayGCType_Int32);
  }
  float                               gc_offset                           ()                                             const
  {
    return get_property_float(vr::Prop_DisplayGCOffset_Float);
  }
  float                               gc_scale                            ()                                             const
  {
    return get_property_float(vr::Prop_DisplayGCScale_Float);
  }
  float                               gc_prescale                         ()                                             const
  {
    return get_property_float(vr::Prop_DisplayGCPrescale_Float);
  }
  std::string                         gc_image_path                       ()                                             const
  {
    return get_property_string(vr::Prop_DisplayGCImage_String);
  }
  float                               gc_black_clamp                      ()                                             const
  {
    return get_property_float(vr::Prop_DisplayGCBlackClamp_Float);
  }
  
  int                                 edid_vendor_id                      ()                                             const
  {
    return get_property_int(vr::Prop_EdidVendorID_Int32);
  }
  int                                 edid_product_id                     ()                                             const
  {
    return get_property_int(vr::Prop_EdidProductID_Int32);
  }
  
  std::uint64_t                       audio_firmware_version              ()                                             const
  {
    return get_property_uint64(vr::Prop_AudioFirmwareVersion_Uint64);
  }
  std::uint64_t                       camera_firmware_version             ()                                             const
  {
    return get_property_uint64(vr::Prop_CameraFirmwareVersion_Uint64);
  }
  std::string                         camera_firmware_description         ()                                             const
  {
    return get_property_string(vr::Prop_CameraFirmwareDescription_String);
  }
  int                                 camera_compatibility_mode           ()                                             const
  {
    return get_property_int(vr::Prop_CameraCompatibilityMode_Int32);
  }
  std::uint64_t                       display_firmware_version            ()                                             const
  {
    return get_property_uint64(vr::Prop_DisplayFirmwareVersion_Uint64);
  }
  
  std::uint64_t                       fpga_version                        ()                                             const
  {
    return get_property_uint64(vr::Prop_DisplayFPGAVersion_Uint64);
  }
  std::uint64_t                       bootloader_version                  ()                                             const
  {
    return get_property_uint64(vr::Prop_DisplayBootloaderVersion_Uint64);
  }
  std::uint64_t                       hardware_version                    ()                                             const
  {
    return get_property_uint64(vr::Prop_DisplayHardwareVersion_Uint64);
  }
  
  std::uint64_t                       graphics_adapter_luid               ()                                             const
  {
    return get_property_uint64(vr::Prop_GraphicsAdapterLuid_Uint64);
  }

  std::string                         chaperone_path                      ()                                             const
  {
    return get_property_string(vr::Prop_DriverProvidedChaperonePath_String);
  }

  // IVR System - Rendering                                                                                           
  std::vector<std::array<float, 2>>   hidden_area_mesh                    (eye eye)                                      const
  {
    const auto hidden_area_mesh = vr::VRSystem()->GetHiddenAreaMesh(static_cast<vr::EVREye>(eye), vr::k_eHiddenAreaMesh_Standard);
    std::vector<std::array<float, 2>> vertices(3 * hidden_area_mesh.unTriangleCount);
    std::copy(&hidden_area_mesh.pVertexData->v[0], &hidden_area_mesh.pVertexData->v[0] + 2 * vertices.size(), &vertices[0][0]);
    return vertices;
  }
  std::vector<std::array<float, 2>>   hidden_area_mesh_inverse            (eye eye)                                      const
  {
    const auto hidden_area_mesh = vr::VRSystem()->GetHiddenAreaMesh(static_cast<vr::EVREye>(eye), vr::k_eHiddenAreaMesh_Inverse);
    std::vector<std::array<float, 2>> vertices(3 * hidden_area_mesh.unTriangleCount);
    std::copy(&hidden_area_mesh.pVertexData->v[0], &hidden_area_mesh.pVertexData->v[0] + 2 * vertices.size(), &vertices[0][0]);
    return vertices;
  }
  std::vector<std::array<float, 2>>   hidden_area_mesh_wireframe          (eye eye)                                      const
  {
    const auto hidden_area_mesh = vr::VRSystem()->GetHiddenAreaMesh(static_cast<vr::EVREye>(eye), vr::k_eHiddenAreaMesh_LineLoop);
    std::vector<std::array<float, 2>> vertices(2 * hidden_area_mesh.unTriangleCount);
    std::copy(&hidden_area_mesh.pVertexData->v[0], &hidden_area_mesh.pVertexData->v[0] + 2 * vertices.size(), &vertices[0][0]);
    return vertices;
  }
};
}

#endif
