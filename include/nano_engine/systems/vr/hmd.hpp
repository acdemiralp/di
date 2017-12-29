#ifndef NANO_ENGINE_SYSTEMS_VR_HMD_HPP_
#define NANO_ENGINE_SYSTEMS_VR_HMD_HPP_

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <openvr.h>

#include <nano_engine/systems/vr/camera_frame_type.hpp>
#include <nano_engine/systems/vr/camera_stream.hpp>
#include <nano_engine/systems/vr/color_space.hpp>
#include <nano_engine/systems/vr/eye.hpp>
#include <nano_engine/systems/vr/mirror_texture_d3d11.hpp>
#include <nano_engine/systems/vr/mirror_texture_opengl.hpp>
#include <nano_engine/systems/vr/statistics.hpp>
#include <nano_engine/systems/vr/submit_flags.hpp>
#include <nano_engine/systems/vr/texture_data_d3d12.hpp>
#include <nano_engine/systems/vr/texture_data_vulkan.hpp>
#include <nano_engine/systems/vr/timing_info.hpp>
#include <nano_engine/systems/vr/timing_mode.hpp>
#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/systems/vr/vr_screenshot.hpp>
#include <nano_engine/systems/vr/vr_screenshot_type.hpp>
#include <nano_engine/utility/rectangle.hpp>

#ifdef _WIN32
#include <winnt.h>
#endif

#undef near
#undef far

typedef struct VkInstance_T*       VkInstance      ;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;

class ID3D11Texture2D;

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
  
  std::chrono::duration<float>           time_to_photons                     ()                                                                                                                                                                                                  const
  {
    return std::chrono::duration<float>(1.0F / display_frequency() - time_since_last_vsync().count() + time_from_vsync_to_photons().count());
  }
                                                                                                                                                                                                                                                                                 
  // IVR System - Display                                                                                                                                                                                                                                                        
  std::array<std::size_t, 2>             recommended_render_target_size      ()                                                                                                                                                                                                  const
  {
    std::array<std::size_t, 2> size;
    vr::VRSystem()->GetRecommendedRenderTargetSize(
      reinterpret_cast<std::uint32_t*>(&size[0]), 
      reinterpret_cast<std::uint32_t*>(&size[1]));
    return size;
  }
  std::array<float, 16>                  projection_matrix                   (eye eye, const float near, const float far)                                                                                                                                                        const
  {
    std::array<float, 16> matrix;
    const auto native_matrix = vr::VRSystem()->GetProjectionMatrix(static_cast<vr::EVREye>(eye), near, far);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 16, matrix.begin());
    return matrix;
  }
  rectangle<float>                       projection_parameters               (eye eye)                                                                                                                                                                                           const
  {
    rectangle<float> parameters;
    vr::VRSystem()->GetProjectionRaw(static_cast<vr::EVREye>(eye), &parameters.left, &parameters.right, &parameters.top, &parameters.bottom);
    return parameters;
  }
  std::array<float, 12>                  eye_to_head_transform               (eye eye)                                                                                                                                                                                           const
  {
    std::array<float, 12> matrix;
    const auto native_matrix = vr::VRSystem()->GetEyeToHeadTransform(static_cast<vr::EVREye>(eye));
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::array<std::array<float, 2>, 3>    compute_distortion                  (eye eye, const std::array<float, 2>& uv)                                                                                                                                                           const
  {
    std::array<std::array<float, 2>, 3> distortion;
    vr::DistortionCoordinates_t native_distortion;
    vr::VRSystem()->ComputeDistortion(static_cast<vr::EVREye>(eye), uv[0], uv[1], &native_distortion);
    distortion[0][0] = native_distortion.rfRed  [0]; distortion[0][1] = native_distortion.rfRed  [1];
    distortion[1][0] = native_distortion.rfGreen[0]; distortion[1][1] = native_distortion.rfGreen[1];
    distortion[2][0] = native_distortion.rfBlue [0]; distortion[2][1] = native_distortion.rfBlue [1];
    return distortion;
  }
                                                                                                                                                                                                                                                                                 
  std::chrono::duration<float>           time_since_last_vsync               ()                                                                                                                                                                                                  const
  {
    float         time       ;
    std::uint64_t frame_count;
    vr::VRSystem()->GetTimeSinceLastVsync(&time, &frame_count);
    return std::chrono::duration<float>(time);
  }
  std::uint64_t                          frame_count                         ()                                                                                                                                                                                                  const
  {
    float         time       ;
    std::uint64_t frame_count;
    vr::VRSystem()->GetTimeSinceLastVsync(&time, &frame_count);
    return frame_count;
  }
                                                                                                                                                                                                                                                                                 
#ifdef _WIN32                                                                                                                                                                                                                                                                    
  std::uint32_t                          adapter_index_d3d9                  ()                                                                                                                                                                                                  const
  {
    return static_cast<std::uint32_t>(vr::VRSystem()->GetD3D9AdapterIndex());
  }
  std::uint32_t                          adapter_index_d3d10_d3d11           ()                                                                                                                                                                                                  const
  {
    std::int32_t index;
    vr::VRSystem()->GetDXGIOutputInfo(&index);
    return static_cast<std::uint32_t>(index);
  }
  LUID                                   output_device_d3d10_d3d11           ()                                                                                                                                                                                                  const
  {
    LUID   luid;
    vr::VRSystem()->GetOutputDevice(reinterpret_cast<std::uint64_t*>(&luid), vr::ETextureType::TextureType_DirectX);
    return luid;
  }
  LUID                                   output_device_d3d12                 ()                                                                                                                                                                                                  const
  {
    LUID   luid;
    vr::VRSystem()->GetOutputDevice(reinterpret_cast<std::uint64_t*>(&luid), vr::ETextureType::TextureType_DirectX12);
    return luid;
  }                                                                                                                                                                                                                                         
#elif __APPLE__                                                                                                                                                                                                                                                                  
  std::uint64_t                          output_device_metal                 ()                                                                                                                                                                                                  const
  {                                                                                                                                                                                                                                                                              
    std::uint64_t id;                                                                                                                                                                                                                                                            
    vr::VRSystem()->GetOutputDevice(&id, vr::ETextureType::TextureType_IOSurface);                                                                                                                                                                                               
    return        id;                                                                                                                                                                                                                                                            
  }                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
#endif                                                                                                                                                                                                                                                                           
  VkPhysicalDevice                       output_device_vulkan                (VkInstance instance)                                                                                                                                                                               const
  {
    VkPhysicalDevice device;
    vr::VRSystem()->GetOutputDevice(reinterpret_cast<std::uint64_t*>(&device), vr::ETextureType::TextureType_Vulkan, instance);
    return           device;
  }
                                                                                                                                                                                                                                                                                 
  // IVR System - Display Mode                                                                                                                                                                                                                                                   
  bool                                   extended_display_mode               ()                                                                                                                                                                                                  const
  {
    return vr::VRSystem()->IsDisplayOnDesktop();
  }
  void                                   set_extended_display_mode           (const bool enabled)
  {
    vr::VRSystem()->SetDisplayVisibility(enabled);
  }                                                                                                                                                                       
                                                                                                                                                                                                                                                                                 
  // IVR System - Tracking                                                                                                                                                                                                                                                       
  void                                   set_current_pose_as_seated_zero_pose()
  {
    vr::VRSystem()->ResetSeatedZeroPose();
  }                                                                                                                                                                                         
  std::array<float, 12>                  seated_zero_pose_to_absolute_pose   ()                                                                                                                                                                                                  const
  {
    std::array<float, 12> matrix;
    const auto native_matrix = vr::VRSystem()->GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::array<float, 12>                  raw_zero_pose_to_absolute_pose      ()                                                                                                                                                                                                  const
  {
    std::array<float, 12> matrix;
    const auto native_matrix = vr::VRSystem()->GetRawZeroPoseToStandingAbsoluteTrackingPose();
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
                                                                                                                                                                                                                                                                                 
  // IVR System - Property                                                                                                                                                                                                                                                       
  std::array<float, 12>                  camera_to_head_transform            ()                                                                                                                                                                                                  const
  {
    return get_property_matrix34(vr::Prop_CameraToHeadTransform_Matrix34);
  } 
  std::array<float, 2>                   lens_center_uv                      (const eye eye)                                                                                                                                                                                     const
  {
    return eye == eye::left ? 
      std::array<float, 2>{get_property_float(vr::Prop_LensCenterLeftU_Float ), get_property_float(vr::Prop_LensCenterLeftV_Float )} :
      std::array<float, 2>{get_property_float(vr::Prop_LensCenterRightU_Float), get_property_float(vr::Prop_LensCenterRightV_Float)} ;
  }
  std::array<float, 2>                   screenshot_field_of_view            ()                                                                                                                                                                                                  const
  {
    return 
    {
      get_property_float(vr::Prop_ScreenshotHorizontalFieldOfViewDegrees_Float),
      get_property_float(vr::Prop_ScreenshotVerticalFieldOfViewDegrees_Float  )
    };
  }
                                                                                                                                                                                                                                                                                 
  float                                  head_to_eye_depth_distance          ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_UserHeadToEyeDepthMeters_Float);
  }
  float                                  interpupillary_distance             ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_UserIpdMeters_Float);
  }
                                                                                                                                                                                                                                                                                 
  std::chrono::duration<float>           time_from_vsync_to_photons          ()                                                                                                                                                                                                  const
  {
    return std::chrono::duration<float>(get_property_float(vr::Prop_SecondsFromVsyncToPhotons_Float));
  }
  std::chrono::duration<float>           time_from_photons_to_vblank         ()                                                                                                                                                                                                  const
  {
    return std::chrono::duration<float>(get_property_float(vr::Prop_SecondsFromPhotonsToVblank_Float));
  }
  float                                  display_frequency                   ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayFrequency_Float);
  }
                                                                                                                                                                                                                                                                                 
  bool                                   direct_mode_sends_vsync_events      ()                                                                                                                                                                                                  const
  {
    return get_property_bool(vr::Prop_DriverDirectModeSendsVsyncEvents_Bool);
  }
  bool                                   reports_time_since_vsync            ()                                                                                                                                                                                                  const
  {
    return get_property_bool(vr::Prop_ReportsTimeSinceVSync_Bool);
  }
  bool                                   on_desktop                          ()                                                                                                                                                                                                  const
  {
    return get_property_bool(vr::Prop_IsOnDesktop_Bool);
  }
  bool                                   prediction                          ()                                                                                                                                                                                                  const
  {
    return !get_property_bool(vr::Prop_DoNotApplyPrediction_Bool);
  }
  bool                                   suppressed                          ()                                                                                                                                                                                                  const
  {
    return get_property_bool(vr::Prop_DisplaySuppressed_Bool);
  }
  bool                                   night_mode_support                  ()                                                                                                                                                                                                  const
  {
    return get_property_bool(vr::Prop_DisplayAllowNightMode_Bool);
  }
  bool                                   debug                               ()                                                                                                                                                                                                  const
  {
    return get_property_bool(vr::Prop_DisplayDebugMode_Bool);
  }
                                                                                                                                                                                                                                                                                 
  std::size_t                            expected_controller_count           ()                                                                                                                                                                                                  const
  {
    return static_cast<std::size_t>(get_property_int(vr::Prop_ExpectedControllerCount_Int32));
  }
  std::size_t                            expected_tracking_reference_count   ()                                                                                                                                                                                                  const
  {
    return static_cast<std::size_t>(get_property_int(vr::Prop_ExpectedTrackingReferenceCount_Int32));
  }
                                                                                                                                                                                                                                                                                 
  std::uint64_t                          current_universe_id                 ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_CurrentUniverseId_Uint64);
  }
  std::uint64_t                          previous_universe_id                ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_PreviousUniverseId_Uint64);
  }
                                                                                                                                                                                                                                                                                 
  int                                    mc_type                             ()                                                                                                                                                                                                  const
  {
    return get_property_int(vr::Prop_DisplayMCType_Int32);
  }
  float                                  mc_offset                           ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayMCOffset_Float);
  }
  float                                  mc_scale                            ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayMCScale_Float);
  }
  std::array<std::size_t, 2>             mc_image_size                       ()                                                                                                                                                                                                  const
  {
    return 
    {
      static_cast<std::size_t>(get_property_int(vr::Prop_DisplayMCImageWidth_Int32 )), 
      static_cast<std::size_t>(get_property_int(vr::Prop_DisplayMCImageHeight_Int32))
    };
  }
  std::size_t                            mc_image_num_channels               ()                                                                                                                                                                                                  const
  {
    return static_cast<std::size_t>(get_property_int(vr::Prop_DisplayMCImageNumChannels_Int32));
  }
  std::string                            mc_image_path                       (const eye eye)                                                                                                                                                                                     const
  {
    return get_property_string(eye == eye::left ? vr::Prop_DisplayMCImageLeft_String : vr::Prop_DisplayMCImageRight_String);
  }
  void*                                  mc_image                            ()                                                                                                                                                                                                  const
  {
    return reinterpret_cast<void*>(get_property_uint64(vr::Prop_DisplayMCImageData_Binary));
  }
                                                                                                                                                                                                                                                                                 
  int                                    gc_type                             ()                                                                                                                                                                                                  const
  {
    return get_property_int(vr::Prop_DisplayGCType_Int32);
  }
  float                                  gc_offset                           ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayGCOffset_Float);
  }
  float                                  gc_scale                            ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayGCScale_Float);
  }
  float                                  gc_prescale                         ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayGCPrescale_Float);
  }
  std::string                            gc_image_path                       ()                                                                                                                                                                                                  const
  {
    return get_property_string(vr::Prop_DisplayGCImage_String);
  }
  float                                  gc_black_clamp                      ()                                                                                                                                                                                                  const
  {
    return get_property_float(vr::Prop_DisplayGCBlackClamp_Float);
  }
                                                                                                                                                                                                                                                                                 
  int                                    edid_vendor_id                      ()                                                                                                                                                                                                  const
  {
    return get_property_int(vr::Prop_EdidVendorID_Int32);
  }
  int                                    edid_product_id                     ()                                                                                                                                                                                                  const
  {
    return get_property_int(vr::Prop_EdidProductID_Int32);
  }
                                                                                                                                                                                                                                                                                 
  std::uint64_t                          audio_firmware_version              ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_AudioFirmwareVersion_Uint64);
  }
  std::uint64_t                          camera_firmware_version             ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_CameraFirmwareVersion_Uint64);
  }
  std::string                            camera_firmware_description         ()                                                                                                                                                                                                  const
  {
    return get_property_string(vr::Prop_CameraFirmwareDescription_String);
  }
  int                                    camera_compatibility_mode           ()                                                                                                                                                                                                  const
  {
    return get_property_int(vr::Prop_CameraCompatibilityMode_Int32);
  }
  std::uint64_t                          display_firmware_version            ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_DisplayFirmwareVersion_Uint64);
  }
                                                                                                                                                                                                                                                                                 
  std::uint64_t                          fpga_version                        ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_DisplayFPGAVersion_Uint64);
  }
  std::uint64_t                          bootloader_version                  ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_DisplayBootloaderVersion_Uint64);
  }
  std::uint64_t                          hardware_version                    ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_DisplayHardwareVersion_Uint64);
  }
                                                                                                                                                                                                                                                                                 
  std::uint64_t                          graphics_adapter_luid               ()                                                                                                                                                                                                  const
  {
    return get_property_uint64(vr::Prop_GraphicsAdapterLuid_Uint64);
  }
                                                                                                                                                                                                                                                                                 
  std::string                            chaperone_path                      ()                                                                                                                                                                                                  const
  {
    return get_property_string(vr::Prop_DriverProvidedChaperonePath_String);
  }
                                                                                                                                                                                                                                                                                 
  // IVR System - Rendering                                                                                                                                                                                                                                                      
  std::vector<std::array<float, 2>>      hidden_area_mesh                    (eye eye)                                                                                                                                                                                           const
  {
    const auto hidden_area_mesh = vr::VRSystem()->GetHiddenAreaMesh(static_cast<vr::EVREye>(eye), vr::k_eHiddenAreaMesh_Standard);
    std::vector<std::array<float, 2>> vertices(3 * hidden_area_mesh.unTriangleCount);
    std::copy(&hidden_area_mesh.pVertexData->v[0], &hidden_area_mesh.pVertexData->v[0] + 2 * vertices.size(), &vertices[0][0]);
    return vertices;
  }
  std::vector<std::array<float, 2>>      hidden_area_mesh_inverse            (eye eye)                                                                                                                                                                                           const
  {
    const auto hidden_area_mesh = vr::VRSystem()->GetHiddenAreaMesh(static_cast<vr::EVREye>(eye), vr::k_eHiddenAreaMesh_Inverse);
    std::vector<std::array<float, 2>> vertices(3 * hidden_area_mesh.unTriangleCount);
    std::copy(&hidden_area_mesh.pVertexData->v[0], &hidden_area_mesh.pVertexData->v[0] + 2 * vertices.size(), &vertices[0][0]);
    return vertices;
  }
  std::vector<std::array<float, 2>>      hidden_area_mesh_wireframe          (eye eye)                                                                                                                                                                                           const
  {
    const auto hidden_area_mesh = vr::VRSystem()->GetHiddenAreaMesh(static_cast<vr::EVREye>(eye), vr::k_eHiddenAreaMesh_LineLoop);
    std::vector<std::array<float, 2>> vertices(2 * hidden_area_mesh.unTriangleCount);
    std::copy(&hidden_area_mesh.pVertexData->v[0], &hidden_area_mesh.pVertexData->v[0] + 2 * vertices.size(), &vertices[0][0]);
    return vertices;
  }
                                         
  // IVR Compositor                      
  void                                   set_paused                          (const bool pause)
  {
    vr::VRCompositor()->SuspendRendering(pause);
  }
  
  void                                   submit                              (eye eye, ID3D11Texture2D*    texture_ptr   , color_space color_space = color_space::automatic, boost::optional<rectangle<float>> rectangle = boost::none, submit_flags flags = submit_flags::none) const
  {
    vr::Texture_t         texture {reinterpret_cast<void*>(texture_ptr), vr::TextureType_DirectX, static_cast<vr::EColorSpace>(color_space)};
    vr::VRTextureBounds_t bounds;
    if (rectangle != boost::none)
      bounds = {rectangle->left, rectangle->bottom, rectangle->right, rectangle->top};
    vr::VRCompositor()->Submit(static_cast<vr::EVREye>(eye), &texture, rectangle != boost::none ? &bounds : nullptr, static_cast<vr::EVRSubmitFlags>(flags));
  }
  void                                   submit                              (eye eye, texture_data_d3d12  texture_data  , color_space color_space = color_space::automatic, boost::optional<rectangle<float>> rectangle = boost::none, submit_flags flags = submit_flags::none) const
  {
    vr::Texture_t         texture {reinterpret_cast<void*>(&texture_data), vr::TextureType_DirectX12, static_cast<vr::EColorSpace>(color_space)};
    vr::VRTextureBounds_t bounds;
    if (rectangle != boost::none)
      bounds = {rectangle->left, rectangle->bottom, rectangle->right, rectangle->top};
    vr::VRCompositor()->Submit(static_cast<vr::EVREye>(eye), &texture, rectangle != boost::none ? &bounds : nullptr, static_cast<vr::EVRSubmitFlags>(flags));
  }
  void                                   submit                              (eye eye, void*               io_surface_ref, color_space color_space = color_space::automatic, boost::optional<rectangle<float>> rectangle = boost::none, submit_flags flags = submit_flags::none) const
  {
    vr::Texture_t         texture {&io_surface_ref, vr::TextureType_IOSurface, static_cast<vr::EColorSpace>(color_space)};
    vr::VRTextureBounds_t bounds;
    if (rectangle != boost::none)
      bounds = {rectangle->left, rectangle->bottom, rectangle->right, rectangle->top};
    vr::VRCompositor()->Submit(static_cast<vr::EVREye>(eye), &texture, rectangle != boost::none ? &bounds : nullptr, static_cast<vr::EVRSubmitFlags>(flags));
  }
  void                                   submit                              (eye eye, std::uint32_t       texture_id    , color_space color_space = color_space::automatic, boost::optional<rectangle<float>> rectangle = boost::none, submit_flags flags = submit_flags::none) const
  {
    vr::Texture_t         texture {reinterpret_cast<void*>(static_cast<std::uintptr_t>(texture_id)), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    vr::VRTextureBounds_t bounds;
    if (rectangle != boost::none)
      bounds = {rectangle->left, rectangle->bottom, rectangle->right, rectangle->top};
    vr::VRCompositor()->Submit(static_cast<vr::EVREye>(eye), &texture, rectangle != boost::none ? &bounds : nullptr, static_cast<vr::EVRSubmitFlags>(flags));
  }
  void                                   submit                              (eye eye, texture_data_vulkan texture_data  , color_space color_space = color_space::automatic, boost::optional<rectangle<float>> rectangle = boost::none, submit_flags flags = submit_flags::none) const
  {
    vr::Texture_t         texture {reinterpret_cast<void*>(&texture_data), vr::TextureType_Vulkan, static_cast<vr::EColorSpace>(color_space)};
    vr::VRTextureBounds_t bounds;
    if (rectangle != boost::none)
      bounds = {rectangle->left, rectangle->bottom, rectangle->right, rectangle->top};
    vr::VRCompositor()->Submit(static_cast<vr::EVREye>(eye), &texture, rectangle != boost::none ? &bounds : nullptr, static_cast<vr::EVRSubmitFlags>(flags));
  } 
  void                                   clear                               ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->ClearLastSubmittedFrame();
  }
                                                                                                                                                                                                                                                                                 
  std::chrono::duration<float>           remaining_frame_time                ()                                                                                                                                                                                                  const
  {
    return std::chrono::duration<float>(vr::VRCompositor()->GetFrameTimeRemaining());
  }
  timing_info                            frame_time_data                     ()                                                                                                                                                                                                  const
  {
    vr::Compositor_FrameTiming frame_timing;
    vr::VRCompositor()->GetFrameTiming(&frame_timing);
    return timing_info {
      static_cast<std::size_t>                (frame_timing.m_nFrameIndex              ),
      static_cast<std::size_t>                (frame_timing.m_nNumFramePresents        ),
      static_cast<std::size_t>                (frame_timing.m_nNumMisPresented         ),
      static_cast<std::size_t>                (frame_timing.m_nNumDroppedFrames        ),
      std::chrono::duration<double>           (frame_timing.m_flSystemTimeInSeconds    ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flPreSubmitGpuMs         ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flPostSubmitGpuMs        ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flTotalRenderGpuMs       ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flCompositorRenderGpuMs  ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flCompositorRenderCpuMs  ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flCompositorIdleCpuMs    ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flClientFrameIntervalMs  ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flPresentCallCpuMs       ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flWaitForPresentCpuMs    ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flSubmitFrameMs          ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flWaitGetPosesCalledMs   ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flNewPosesReadyMs        ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flNewFrameReadyMs        ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flCompositorUpdateStartMs),
      std::chrono::duration<float, std::milli>(frame_timing.m_flCompositorUpdateEndMs  ),
      std::chrono::duration<float, std::milli>(frame_timing.m_flCompositorRenderStartMs)
    };
  }
  statistics                             cumulative_statistics               ()                                                                                                                                                                                                  const 
  {
    vr::Compositor_CumulativeStats stats;
    vr::VRCompositor()->GetCumulativeStats(&stats, sizeof vr::Compositor_CumulativeStats);
    return statistics {
      static_cast<std::size_t>(stats.m_nPid                           ),
      static_cast<std::size_t>(stats.m_nNumFramePresents              ),
      static_cast<std::size_t>(stats.m_nNumDroppedFrames              ),
      static_cast<std::size_t>(stats.m_nNumReprojectedFrames          ),
      static_cast<std::size_t>(stats.m_nNumFramePresentsOnStartup     ),
      static_cast<std::size_t>(stats.m_nNumDroppedFramesOnStartup     ),
      static_cast<std::size_t>(stats.m_nNumReprojectedFramesOnStartup ),
      static_cast<std::size_t>(stats.m_nNumLoading                    ),
      static_cast<std::size_t>(stats.m_nNumFramePresentsLoading       ),
      static_cast<std::size_t>(stats.m_nNumDroppedFramesLoading       ),
      static_cast<std::size_t>(stats.m_nNumReprojectedFramesLoading   ),
      static_cast<std::size_t>(stats.m_nNumTimedOut                   ),
      static_cast<std::size_t>(stats.m_nNumFramePresentsTimedOut      ),
      static_cast<std::size_t>(stats.m_nNumDroppedFramesTimedOut      ),
      static_cast<std::size_t>(stats.m_nNumReprojectedFramesTimedOut  )
    };
  }
                                                                                                                                                                                                                                                                                 
  void                                   set_timing_mode                     (timing_mode timing_mode)
  {
    vr::VRCompositor()->SetExplicitTimingMode(static_cast<vr::EVRCompositorTimingMode>(timing_mode));
  }                                                                                                                                                                  
  void                                   submit_explicit_timing_data         ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->SubmitExplicitTimingData();
  }                                                                                                                         
  void                                   post_present_handoff                ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->PostPresentHandoff();
  }
                                                                                                                                                                                                                                                                                 
  void                                   set_mirror_window_visible           (const bool visible)
  {
    visible ? vr::VRCompositor()->ShowMirrorWindow() : vr::VRCompositor()->HideMirrorWindow();
  }                               
  bool                                   mirror_window_visible               ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->IsMirrorWindowVisible();
  }
  std::unique_ptr<mirror_texture_d3d11>  mirror_texture_d3d11                (eye eye)                                                                                                                                                                                           const
  {
    return std::make_unique<ne::mirror_texture_d3d11>(eye);
  }
  std::unique_ptr<mirror_texture_opengl> mirror_texture_opengl               (eye eye)                                                                                                                                                                                           const
  {
    return std::make_unique<ne::mirror_texture_opengl>(eye);
  }
             
  void                                   fade_to_color                       (std::array<float, 4> color  , std::chrono::duration<float> duration = std::chrono::duration<float>(1.0F), const bool background = false)                                                           const
  {                                                
    vr::VRCompositor()->FadeToColor(duration.count(), color[0], color[1], color[2], color[3], background);
  }                                                
  std::array<float, 4>                   fade_color                          (                                                                                                          const bool background = false)                                                           const
  {                                                
    const auto native_color = vr::VRCompositor()->GetCurrentFadeColor(background);
    return {native_color.r, native_color.g, native_color.b, native_color.a};
  }
  void                                   fade_grid                           (const bool           fade_in, std::chrono::duration<float> duration = std::chrono::duration<float>(1.0F))                                                                                          const
  {                                                
    vr::VRCompositor()->FadeGrid(duration.count(), fade_in);
  }                                                
  float                                  fade_grid_alpha                     ()                                                                                                                                                                                                  const
  {                                                                                                                                        
    return vr::VRCompositor()->GetCurrentGridAlpha();                                                                                                 
  }                                                                                                                                        
   
  void                                   set_skybox                          (ID3D11Texture2D*                   texture_ptr    , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(texture_ptr), vr::TextureType_DirectX, static_cast<vr::EColorSpace>(color_space)};
    vr::VRCompositor()->SetSkyboxOverride(&texture, 1);
  }
  void                                   set_skybox                          (texture_data_d3d12                 texture_data   , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(&texture_data), vr::TextureType_DirectX12, static_cast<vr::EColorSpace>(color_space)};
    vr::VRCompositor()->SetSkyboxOverride(&texture, 1);
  }
  void                                   set_skybox                          (void*                              io_surface_ref , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {io_surface_ref, vr::TextureType_IOSurface, static_cast<vr::EColorSpace>(color_space)};
    vr::VRCompositor()->SetSkyboxOverride(&texture, 1);
  }
  void                                   set_skybox                          (std::uint32_t                      texture_id     , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(static_cast<std::uintptr_t>(texture_id)), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    vr::VRCompositor()->SetSkyboxOverride(&texture, 1);
  }
  void                                   set_skybox                          (texture_data_vulkan                texture_data   , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(&texture_data), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    vr::VRCompositor()->SetSkyboxOverride(&texture, 1);
  }
  void                                   set_skybox                          (std::array<ID3D11Texture2D*   , 2> texture_ptrs   , color_space color_space = color_space::automatic)
  {
    std::array<vr::Texture_t, 2> textures;
    std::transform(texture_ptrs.begin(), texture_ptrs.end(), textures.begin(), [&color_space] (ID3D11Texture2D* iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(iteratee), vr::TextureType_DirectX, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));
  }
  void                                   set_skybox                          (std::array<texture_data_d3d12 , 2> texture_data   , color_space color_space = color_space::automatic)
  {
    std::array<vr::Texture_t, 2> textures;
    std::transform(texture_data.begin(), texture_data.end(), textures.begin(), [&color_space] (texture_data_d3d12& iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(&iteratee), vr::TextureType_DirectX12, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));
  }
  void                                   set_skybox                          (std::array<void*              , 2> io_surface_refs, color_space color_space = color_space::automatic)
  {
    std::array<vr::Texture_t, 2> textures;
    std::transform(io_surface_refs.begin(), io_surface_refs.end(), textures.begin(), [&color_space] (void* iteratee)
    {
      return vr::Texture_t{iteratee, vr::TextureType_IOSurface, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));
  }
  void                                   set_skybox                          (std::array<std::uint32_t      , 2> texture_ids    , color_space color_space = color_space::automatic)
  {  
    std::array<vr::Texture_t, 2> textures;
    std::transform(texture_ids.begin(), texture_ids.end(), textures.begin(), [&color_space] (std::uint32_t iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(static_cast<std::uintptr_t>(iteratee)), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));                                                                                                                     
  }                                                                                                                                                           
  void                                   set_skybox                          (std::array<texture_data_vulkan, 2> texture_data   , color_space color_space = color_space::automatic)
  {                  
    std::array<vr::Texture_t, 2> textures;
    std::transform(texture_data.begin(), texture_data.end(), textures.begin(), [&color_space] (texture_data_vulkan& iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(&iteratee), vr::TextureType_Vulkan, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));                                                                                                          
  }                                                                                                                                                           
  void                                   set_skybox                          (std::array<ID3D11Texture2D*   , 6> texture_ptrs   , color_space color_space = color_space::automatic)
  {  
    std::array<vr::Texture_t, 6> textures;
    std::transform(texture_ptrs.begin(), texture_ptrs.end(), textures.begin(), [&color_space] (ID3D11Texture2D* iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(iteratee), vr::TextureType_DirectX, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));                                                                                                                          
  }                                                                                                                                                           
  void                                   set_skybox                          (std::array<texture_data_d3d12 , 6> texture_data   , color_space color_space = color_space::automatic)
  { 
    std::array<vr::Texture_t, 6> textures;
    std::transform(texture_data.begin(), texture_data.end(), textures.begin(), [&color_space] (texture_data_d3d12& iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(&iteratee), vr::TextureType_DirectX12, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));                                                                                                                          
  }                                                                                                                                                           
  void                                   set_skybox                          (std::array<void*              , 6> io_surface_refs, color_space color_space = color_space::automatic)
  {                                                                                                                             
    std::array<vr::Texture_t, 6> textures;
    std::transform(io_surface_refs.begin(), io_surface_refs.end(), textures.begin(), [&color_space] (void* iteratee)
    {
      return vr::Texture_t{iteratee, vr::TextureType_IOSurface, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));                                                                                                                             
  }                                                                                                                                                           
  void                                   set_skybox                          (std::array<std::uint32_t      , 6> texture_ids    , color_space color_space = color_space::automatic)
  {      
    std::array<vr::Texture_t, 6> textures;
    std::transform(texture_ids.begin(), texture_ids.end(), textures.begin(), [&color_space] (std::uint32_t iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(static_cast<std::uintptr_t>(iteratee)), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));                                                                                                                     
  }                                                                                                                                                           
  void                                   set_skybox                          (std::array<texture_data_vulkan, 6> texture_data   , color_space color_space = color_space::automatic)
  {
    std::array<vr::Texture_t, 6> textures;
    std::transform(texture_data.begin(), texture_data.end(), textures.begin(), [&color_space] (texture_data_vulkan& iteratee)
    {
      return vr::Texture_t{reinterpret_cast<void*>(&iteratee), vr::TextureType_Vulkan, static_cast<vr::EColorSpace>(color_space)};
    });
    vr::VRCompositor()->SetSkyboxOverride(textures.data(), static_cast<std::uint32_t>(textures.size()));
  }
  void                                   clear_skybox                        ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->ClearSkyboxOverride();
  }
                                                                                                                                                                                                                                                                                 
  void                                   to_front                            ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->CompositorBringToFront();
  }
  void                                   to_back                             ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->CompositorGoToBack();
  }
  void                                   dump_images                         ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->CompositorDumpImages();
  }
                                                                                                                                                                                                                                                                                     
  bool                                   is_fullscreen                       ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->IsFullscreen();
  }                                
  bool                                   can_render                          ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->CanRenderScene();
  }
  bool                                   low_resources                       ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->ShouldAppRenderWithLowResources();
  }
                                                                                                                                                                                                                                                                                 
  std::uint32_t                          process_id                          ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->GetCurrentSceneFocusProcess();
  }
  std::uint32_t                          last_process_id                     ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->GetLastFrameRenderer();
  }
                                                                                                                                                                                                                                                                                 
  std::vector<std::string>               required_vulkan_instance_extensions ()                                                                                                                                                                                                  const
  {
    std::vector<std::string> extensions;
    char extensions_string[vr::k_unMaxPropertyStringSize];
    vr::VRCompositor()->GetVulkanInstanceExtensionsRequired(extensions_string, vr::k_unMaxPropertyStringSize);
    boost::algorithm::split(extensions, extensions_string, boost::is_any_of(" "));
    return extensions;
  }                                      
  std::vector<std::string>               required_vulkan_device_extensions   (VkPhysicalDevice_T* device)                                                                                                                                                                        const
  {
    std::vector<std::string> extensions;
    char extensions_string[vr::k_unMaxPropertyStringSize];
    vr::VRCompositor()->GetVulkanDeviceExtensionsRequired(device, extensions_string, vr::k_unMaxPropertyStringSize);
    boost::algorithm::split(extensions, extensions_string, boost::is_any_of(" "));
    return extensions;
  }
                                                                                                                                                                                                                                                                                 
  void                                   force_interleaved_projection        (const bool enabled)                                                                                                                                                                                const
  {
    vr::VRCompositor()->ForceInterleavedReprojectionOn(enabled);
  }               
  void                                   force_reconnect_to_process          ()                                                                                                                                                                                                  const
  {
    vr::VRCompositor()->ForceReconnectProcess();
  }                                 
  void                                   force_quit                          ()                                                                                                                                                                                                  const
  {
    return vr::VRCompositor()->CompositorQuit();
  }
                                                                                                                                                                                                                       
  // IVR Tracked Camera                                                                                                                                                                                                
  bool                                   has_camera                          ()                                                                                                                                                                                                  const
  {
    bool   has_camera;
    vr::VRTrackedCamera()->HasCamera(index_, &has_camera);
    return has_camera;
  }
  std::array<std::size_t, 2>             camera_frame_size                   (                                   camera_frame_type type = camera_frame_type::distorted)                                                                                                          const
  {
    std::uint32_t width, height, framebuffer_size;
    vr::VRTrackedCamera()->GetCameraFrameSize(index_, static_cast<vr::EVRTrackedCameraFrameType>(type), &width, &height, &framebuffer_size);
    return std::array<std::size_t, 2>{static_cast<std::size_t>(width), static_cast<std::size_t>(height)};
  }
  std::size_t                            camera_framebuffer_size             (                                   camera_frame_type type = camera_frame_type::distorted)                                                                                                          const
  {
    std::uint32_t width, height, framebuffer_size;
    vr::VRTrackedCamera()->GetCameraFrameSize(index_, static_cast<vr::EVRTrackedCameraFrameType>(type), &width, &height, &framebuffer_size);
    return static_cast<std::size_t>(framebuffer_size);
  }
  std::array<float, 2>                   camera_focal_length                 (                                   camera_frame_type type = camera_frame_type::distorted)                                                                                                          const
  {
    vr::HmdVector2_t focal_length, center;
    vr::VRTrackedCamera()->GetCameraIntrinsics(index_, static_cast<vr::EVRTrackedCameraFrameType>(type), &focal_length, &center);
    return {focal_length.v[0], focal_length.v[1]};
  }
  std::array<float, 2>                   camera_center                       (                                   camera_frame_type type = camera_frame_type::distorted)                                                                                                          const
  {
    vr::HmdVector2_t focal_length, center;
    vr::VRTrackedCamera()->GetCameraIntrinsics(index_, static_cast<vr::EVRTrackedCameraFrameType>(type), &focal_length, &center);
    return {center.v[0], center.v[1]};
  }
  std::array<float, 16>                  camera_projection_matrix            (const float near, const float far, camera_frame_type type = camera_frame_type::distorted)                                                                                                          const
  {
    std::array<float, 16> matrix       ;
    vr::HmdMatrix44_t     native_matrix;
    vr::VRTrackedCamera()->GetCameraProjection(index_, static_cast<vr::EVRTrackedCameraFrameType>(type), near, far, &native_matrix);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 16, matrix.begin());
    return matrix;
  }
  std::unique_ptr<camera_stream>         camera_stream                       (                                   camera_frame_type type = camera_frame_type::distorted)                                                                                                          const
  {
    vr::TrackedCameraHandle_t handle;
    vr::VRTrackedCamera()->AcquireVideoStreamingService(index_, &handle);
    return std::make_unique<ne::camera_stream>(handle);
  }

  // IVR Screenshots                                                                                                                                                                                                                                                             
  void                                   obtain_screenshot_focus             ()                                                                                                                                                                                                  const
  {
    std::vector<vr::EVRScreenshotType> types 
    {
      vr::VRScreenshotType_None          ,
      vr::VRScreenshotType_Mono          ,
      vr::VRScreenshotType_Stereo        ,
      vr::VRScreenshotType_Cubemap       ,
      vr::VRScreenshotType_MonoPanorama  ,
      vr::VRScreenshotType_StereoPanorama
    };
    vr::VRScreenshots()->HookScreenshot(types.data(), static_cast<int>(types.size()));
  }
  vr_screenshot                          create_screenshot                   (vr_screenshot_type type, const std::string& name, const std::string& preview_name = "")                                                                                                            const
  {
    vr::ScreenshotHandle_t handle;
    vr::VRScreenshots()->RequestScreenshot(&handle, static_cast<vr::EVRScreenshotType>(type), !preview_name.empty() ? preview_name.c_str() : (name + "_preview").c_str(), name.c_str());
    return vr_screenshot(handle);
  }
  vr_screenshot                          create_quick_screenshot             (                         const std::string& name, const std::string& preview_name = "")                                                                                                            const
  {
    vr::ScreenshotHandle_t handle;
    vr::VRScreenshots()->TakeStereoScreenshot(&handle, !preview_name.empty() ? preview_name.c_str() : (name + "_preview").c_str(), name.c_str());
    return vr_screenshot(handle);
  }
};
}

#endif
