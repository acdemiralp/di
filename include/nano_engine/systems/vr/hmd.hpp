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
  explicit hmd  (const std::uint32_t index) : tracking_device<tracking_device_type::hmd>(index)
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
  bool  reports_time_since_vsync     () const
  {
    return get_property_bool(vr::Prop_ReportsTimeSinceVSync_Bool);
  }
  float seconds_from_vsync_to_photons() const
  {
    return get_property_float(vr::Prop_SecondsFromVsyncToPhotons_Float);
  }
  float display_frequency            () const
  {
    return get_property_float(vr::Prop_DisplayFrequency_Float);
  }
  float user_ipd_meters              () const
  {
    return get_property_float(vr::Prop_UserIpdMeters_Float);
  }
  std::uint64_t current_universe_id() const
  {
    
  }
  std::uint64_t previous_universe_id() const
  {
    
  }
  std::uint64_t display_firmware_version() const
  {
    
  }
  bool is_on_desktop() const
  {
    
  }
  int mc_type() const
  {
    
  }
  float mc_offset() const
  {
    
  }
  float mc_scale() const
  {
    
  }
  int edid_vendor_id() const
  {
    
  }
  std::string mc_image_left() const
  {
    
  }
  std::string mc_image_right() const
  {
    
  }
  float gc_black_clamp() const
  {
    
  }
  int edid_product_id() const
  {
    
  }
  std::array<float, 12> camera_to_head_transform() const
  {
    
  }
  int gc_type() const
  {
    
  }
  float gc_offset() const
  {
    
  }
  float gc_scale() const
  {
    
  }
  float gc_prescale() const
  {
    
  }
  std::string gc_image() const
  {
    
  }
  float lens_center_left_u() const
  {
    
  }
  float lens_center_right_u() const
  {
    
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
