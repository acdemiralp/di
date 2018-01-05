#ifndef NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include <boost/optional.hpp>
#include <openvr.h>

#include <nano_engine/systems/vr/color_space.hpp>
#include <nano_engine/systems/vr/direction.hpp>
#include <nano_engine/systems/vr/hand.hpp>
#include <nano_engine/systems/vr/raycast_result.hpp>
#include <nano_engine/systems/vr/texture_data_d3d12.hpp>
#include <nano_engine/systems/vr/texture_data_vulkan.hpp>
#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/systems/vr/tracking_device_type.hpp>
#include <nano_engine/systems/vr/tracking_mode.hpp>
#include <nano_engine/utility/rectangle.hpp>

#undef min
#undef max

class ID3D11Texture2D;

namespace ne
{
class overlay
{
public:
  explicit overlay  (const std::string& key, const std::string& name)
  {
    vr::VROverlay()->CreateOverlay(key.c_str(), name.c_str(), &id_);
  }
  overlay           (const overlay&  that) = delete ;
  overlay           (      overlay&& temp) noexcept : id_(std::move(temp.id_))
  {
    temp.id_ = invalid_id;
  }
  virtual ~overlay  ()
  {
    if(id_ != invalid_id)
      vr::VROverlay()->DestroyOverlay(id_);
  }
  overlay& operator=(const overlay&  that) = delete ;
  overlay& operator=(      overlay&& temp) noexcept
  {
    if (this != &temp)
    {
      id_ = std::move(temp.id_);

      temp.id_ = invalid_id;
    }
    return *this;
  }

  // IVR Applications
  void                       launch                        () const
  {
    vr::VRApplications()->LaunchDashboardOverlay(key().c_str());
  }

  // IVR Overlay - Management
  static overlay             find                          (const std::string& key )
  {
    std::uint64_t handle;
    vr::VROverlay()->FindOverlay(key.c_str(), &handle);
    return overlay(handle);
  }

  void                       set_name                      (const std::string& name)        
  {
    vr::VROverlay()->SetOverlayName(id_, name.c_str());
  }
  void                       set_high_quality              ()
  {
    vr::VROverlay()->SetHighQualityOverlay(id_);
  }

  std::string                key                           () const
  {
    char key[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayKey(id_, key, vr::k_unMaxPropertyStringSize);
    return std::string(key);
  }
  std::string                name                          () const
  {
    char name[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayName(id_, name, vr::k_unMaxPropertyStringSize);
    return std::string(name);
  }
  bool                       is_high_quality               () const
  {
    return vr::VROverlay()->GetHighQualityOverlay() == id_;
  }
  std::array<std::size_t, 2> image_size                    () const
  {
    std::array<std::uint32_t, 2> size;
    vr::VROverlay()->GetOverlayImageData(id_, nullptr, 0, &size[0], &size[1]);
    return {static_cast<std::size_t>(size[0]), static_cast<std::size_t>(size[1])};
  }
  std::vector<std::uint8_t>  image_data                    () const
  {
    auto size = image_size();
    std::vector<std::uint8_t> data(4 * size[0] * size[1]);
    vr::VROverlay()->GetOverlayImageData(id_, data.data(), static_cast<std::uint32_t>(data.size()), nullptr, nullptr);
    return data;
  }

  // IVR Overlay - Rendering                               
  void                       set_rendering_pid             (const std::uint32_t          rendering_pid     )        
  {
    vr::VROverlay()->SetOverlayRenderingPid(id_, rendering_pid);
  }
  void                       set_curved                    (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_Curved, enabled);
  }
  void                       set_rotated_grid_supersampling(const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_RGSS4X, enabled);
  }
  void                       set_gamepad_focus_highlighting(const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_ShowGamepadFocus, enabled);
  }
  void                       set_scroll_events             (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SendVRScrollEvents, enabled);
  }
  void                       set_touchpad_events           (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SendVRTouchpadEvents, enabled);
  }
  void                       set_scroll_wheel_visible      (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_ShowTouchPadScrollWheel, enabled);
  }
  void                       set_transferable              (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_TransferOwnershipToInternalProcess, enabled);
  }
  void                       set_texture_parallel          (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SideBySide_Parallel, enabled);
  }
  void                       set_texture_crossed           (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SideBySide_Crossed, enabled);
  }
  void                       set_panorama                  (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_Panorama, enabled);
  }
  void                       set_stereo_panorama           (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_StereoPanorama, enabled);
  }
  void                       set_global_sorting            (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SortWithNonSceneOverlays, enabled);
  }
  void                       set_visible_in_dashboard      (const bool                   enabled           )
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_VisibleInDashboard, enabled);
  }
  void                       set_color                     (const std::array<float, 4>&  color             )
  {
    vr::VROverlay()->SetOverlayColor(id_, color[0], color[1], color[2]);
    vr::VROverlay()->SetOverlayAlpha(id_, color[2]);
  }
  void                       set_texel_aspect_ratio        (const float                  texel_aspect_ratio) 
  {
    vr::VROverlay()->SetOverlayTexelAspect(id_, texel_aspect_ratio);
  }
  void                       set_sort_order                (const std::uint32_t          sort_order        ) 
  {
    vr::VROverlay()->SetOverlaySortOrder(id_, sort_order);
  }
  void                       set_width                     (const float                  width             ) 
  {
    vr::VROverlay()->SetOverlayWidthInMeters(id_, width);
  }
  void                       set_auto_curve_range          (const std::array<float, 2>&  auto_curve_range  ) 
  {
    vr::VROverlay()->SetOverlayAutoCurveDistanceRangeInMeters(id_, auto_curve_range[0], auto_curve_range[1]);
  }
  void                       set_texture_color_space       (const color_space            color_space       )
  {
    vr::VROverlay()->SetOverlayTextureColorSpace(id_, static_cast<vr::EColorSpace>(color_space));
  }
  void                       set_texture_bounds            (const rectangle<float>&      texture_bounds    )
  {
    vr::VRTextureBounds_t bounds {texture_bounds.left, texture_bounds.bottom, texture_bounds.right, texture_bounds.top};
    vr::VROverlay()->SetOverlayTextureBounds(id_, &bounds);
  }
  void                       set_model                     (const std::string&           model_name        , boost::optional<std::array<float, 4>> color = boost::none             )
  {
    vr::HmdColor_t native_color;
    if (color != boost::none)
      native_color = {color->at(0), color->at(1), color->at(2), color->at(3)};
    vr::VROverlay()->SetOverlayRenderModel(id_, model_name.c_str(), color != boost::none ? &native_color : nullptr);
  }
  void                       set_transform                 (const std::array<float, 12>& matrix            , tracking_mode                         mode  = tracking_mode::automatic)
  {
    if (mode == tracking_mode::automatic)
      mode = static_cast<tracking_mode>(vr::VRCompositor()->GetTrackingSpace());

    vr::HmdMatrix34_t native_matrix;
    std::copy(matrix.begin(), matrix.end(), &native_matrix.m[0][0]);
    vr::VROverlay()->SetOverlayTransformAbsolute(id_, static_cast<vr::ETrackingUniverseOrigin>(mode), &native_matrix);
  }
  void                       set_transform                 (const std::array<float, 12>& matrix            , const overlay&                        parent                          )
  {
    vr::HmdMatrix34_t native_matrix;
    std::copy(matrix.begin(), matrix.end(), &native_matrix.m[0][0]);
    vr::VROverlay()->SetOverlayTransformOverlayRelative(id_, parent.id_, &native_matrix);
  }
  template <tracking_device_type type>                     
  void                       set_transform                 (const std::array<float, 12>& matrix            , const tracking_device<type>&          device                          )
  {
    vr::HmdMatrix34_t native_matrix;
    std::copy(matrix.begin(), matrix.end(), &native_matrix.m[0][0]);
    vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(id_, device.index(), &native_matrix);
  }
  template <tracking_device_type type>                     
  void                       attach_to_component           (const tracking_device<type>& device            , const std::string&                    component_name                  )
  {
    vr::VROverlay()->SetOverlayTransformTrackedDeviceComponent(id_, device.index(), component_name.c_str());
  }
  void                       set_visible                   (const bool                   visible           ) 
  {
    visible ? vr::VROverlay()->ShowOverlay(id_) : vr::VROverlay()->HideOverlay(id_);
  }

  std::uint32_t              rendering_pid                 () const
  {
    return vr::VROverlay()->GetOverlayRenderingPid(id_);
  }
  bool                       curved                        () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_Curved, &enabled);
    return enabled;
  }
  bool                       rotated_grid_supersampling    () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_RGSS4X, &enabled);
    return enabled;
  }
  bool                       gamepad_focus_highlighting    () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_ShowGamepadFocus, &enabled);
    return enabled;
  }
  bool                       scroll_events                 () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SendVRScrollEvents, &enabled);
    return enabled;
  }
  bool                       touchpad_events               () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SendVRTouchpadEvents, &enabled);
    return enabled;
  }
  bool                       scroll_wheel_visible          () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_ShowTouchPadScrollWheel, &enabled);
    return enabled;
  }
  bool                       transferable                  () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_TransferOwnershipToInternalProcess, &enabled);
    return enabled;
  }
  bool                       texture_parallel              () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SideBySide_Parallel, &enabled);
    return enabled;
  }
  bool                       texture_crossed               () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SideBySide_Crossed, &enabled);
    return enabled;
  }
  bool                       panorama                      () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_Panorama, &enabled);
    return enabled;
  }
  bool                       stereo_panorama               () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_StereoPanorama, &enabled);
    return enabled;
  }
  bool                       global_sorting                () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_SortWithNonSceneOverlays, &enabled);
    return enabled;
  }
  bool                       visible_in_dashboard          () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_VisibleInDashboard, &enabled);
    return enabled;
  }
  std::array<float, 4>       color                         () const
  {
    std::array<float, 4> color;
    vr::VROverlay()->GetOverlayColor(id_, &color[0], &color[1], &color[2]);
    vr::VROverlay()->GetOverlayAlpha(id_, &color[3]);
    return color;
  }
  float                      texel_aspect_ratio            () const
  {
    float  texel_aspect_ratio;
    vr::VROverlay()->GetOverlayTexelAspect(id_, &texel_aspect_ratio);
    return texel_aspect_ratio;
  }
  std::uint32_t              sort_order                    () const
  {
    std::uint32_t sort_order;
    vr::VROverlay()->GetOverlaySortOrder(id_, &sort_order);
    return        sort_order;
  }
  float                      width                         () const
  {
    float  width;
    vr::VROverlay()->GetOverlayWidthInMeters(id_, &width);
    return width;
  }
  std::array<float, 2>       auto_curve_range              () const
  {
    std::array<float, 2> auto_curve_range;
    vr::VROverlay()->GetOverlayAutoCurveDistanceRangeInMeters(id_, &auto_curve_range[0], &auto_curve_range[1]);
    return               auto_curve_range;
  }
  color_space                texture_color_space           () const
  {
    vr::EColorSpace color_space;
    vr::VROverlay()->GetOverlayTextureColorSpace(id_, &color_space);
    return static_cast<ne::color_space>(color_space);
  }
  rectangle<float>           texture_bounds                () const
  {
    vr::VRTextureBounds_t texture_bounds;
    vr::VROverlay()->SetOverlayTextureBounds(id_, &texture_bounds);
    return rectangle<float> {texture_bounds.uMin, texture_bounds.vMin, texture_bounds.uMax, texture_bounds.vMax};
  }
  std::string                model_name                    () const
  {
    char                render_model_name[vr::k_unMaxPropertyStringSize];
    vr::HmdColor_t      color;
    vr::EVROverlayError error;
    vr::VROverlay()->GetOverlayRenderModel(id_, render_model_name, vr::k_unMaxPropertyStringSize, &color, &error);
    return std::string(render_model_name);
  }
  std::array<float, 4>       model_color                   () const
  {
    char                render_model_name[vr::k_unMaxPropertyStringSize];
    vr::HmdColor_t      color;
    vr::EVROverlayError error;
    vr::VROverlay()->GetOverlayRenderModel(id_, render_model_name, vr::k_unMaxPropertyStringSize, &color, &error);
    return {color.r, color.g, color.b, color.a};
  }
  std::array<float, 12>      transform                     () const
  {
    vr::VROverlayTransformType type;
    vr::VROverlay()->GetOverlayTransformType(id_, &type);

    vr::HmdMatrix34_t     native_matrix;
    std::array<float, 12> matrix       ;
    if     (type == vr::VROverlayTransform_Absolute)
    {
      vr::ETrackingUniverseOrigin mode;
      vr::VROverlay()->GetOverlayTransformAbsolute(id_, &mode, &native_matrix);
    }
    else if(type == vr::VROverlayTransform_SystemOverlay)
    {
      std::uint64_t parent_index;
      vr::VROverlay()->GetOverlayTransformOverlayRelative(id_, &parent_index, &native_matrix);
    }
    else if(type == vr::VROverlayTransform_TrackedDeviceRelative)
    {
      std::uint32_t device_index;
      vr::VROverlay()->GetOverlayTransformTrackedDeviceRelative(id_, &device_index, &native_matrix);
    }
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::array<float, 12>      coordinate_transform          (const std::array<float, 2> position, tracking_mode mode = tracking_mode::automatic)
  {
    if (mode == tracking_mode::automatic)
      mode = static_cast<tracking_mode>(vr::VRCompositor()->GetTrackingSpace());
    
    vr::HmdVector2_t      native_position {position[0], position[1]};
    vr::HmdMatrix34_t     native_matrix;
    std::array<float, 12> matrix       ;
    vr::VROverlay()->GetTransformForOverlayCoordinates(id_, static_cast<vr::ETrackingUniverseOrigin>(mode), native_position, &native_matrix);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::uint32_t              attached_device_index         () const
  {
    std::uint32_t id;
    char name[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayTransformTrackedDeviceComponent(id_, &id, name, vr::k_unMaxPropertyStringSize);
    return id;
  }
  std::string                attached_component_name       () const
  {
    std::uint32_t id;
    char name[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayTransformTrackedDeviceComponent(id_, &id, name, vr::k_unMaxPropertyStringSize);
    return std::string(name);
  }
  bool                       visible                       () const
  {
    return vr::VROverlay()->IsOverlayVisible(id_);
  }

  // IVR Overlay - Input
  void                       process_events                () const
  {
    vr::VREvent_t event;
    while (vr::VROverlay()->PollNextOverlayEvent(id_, &event, sizeof vr::VREvent_t))
    {
      // TODO: Process events.
    }
  }

  void                       set_input_enabled             (const bool enabled)
  {
    vr::VROverlay()->SetOverlayInputMethod(id_, enabled ? vr::VROverlayInputMethod_Mouse : vr::VROverlayInputMethod_None);
  }
  void                       set_dual_analog_mode          ()
  {
    vr::VROverlay()->SetOverlayInputMethod(id_, vr::VROverlayInputMethod_DualAnalog);
  }
  void                       set_mouse_mode                ()
  {
    vr::VROverlay()->SetOverlayInputMethod(id_, vr::VROverlayInputMethod_Mouse);
  }
  template <tracking_device_type type>
  void                       set_mouse_device              (const tracking_device<type>& device     )
  {
    vr::VROverlay()->HandleControllerOverlayInteractionAsMouse(id_, device.index());
  }
  void                       set_mouse_scale               (const std::array<float, 2>&  mouse_scale) 
  {
    vr::HmdVector2_t scale {{mouse_scale[0], mouse_scale[1]}};
    vr::VROverlay()->SetOverlayMouseScale(id_, &scale);
  }
  void                       set_gamepad_focus             ()
  {
    vr::VROverlay()->SetGamepadFocusOverlay(id_);
  }
  void                       set_dual_analog_transform     (const hand hand, const std::array<float, 2>& center, const float radius)
  {
    const vr::HmdVector2_t native_center {{center[0], center[1]}};
    vr::VROverlay()->SetOverlayDualAnalogTransform(id_, hand == hand::left ? vr::k_EDualAnalog_Left : vr::k_EDualAnalog_Right, native_center, radius);
  }

  std::array<float, 2>       mouse_scale                   () const
  {
    vr::HmdVector2_t mouse_scale;
    vr::VROverlay()->GetOverlayMouseScale(id_, &mouse_scale);
    return {mouse_scale.v[0], mouse_scale.v[1]};
  }
  bool                       gamepad_focus                 () const
  {
    return vr::VROverlay()->GetGamepadFocusOverlay() == id_;
  }
  bool                       is_hover_target_display       () const
  {
    return vr::VROverlay()->IsHoverTargetOverlay(id_);
  }
  std::array<float, 2>       dual_analog_center            (const hand hand) const
  {
    vr::HmdVector2_t center;
    float            radius;
    vr::VROverlay()->GetOverlayDualAnalogTransform(id_, hand == hand::left ? vr::k_EDualAnalog_Left : vr::k_EDualAnalog_Right, &center, &radius);
    return {center.v[0], center.v[1]};
  }
  float                      dual_analog_radius            (const hand hand) const
  {
    vr::HmdVector2_t center;
    float            radius;
    vr::VROverlay()->GetOverlayDualAnalogTransform(id_, hand == hand::left ? vr::k_EDualAnalog_Left : vr::k_EDualAnalog_Right, &center, &radius);
    return radius;
  }

  void                       set_neighbor                  (const direction direction, const overlay& neighbor)
  {
    vr::VROverlay()->SetOverlayNeighbor(static_cast<vr::EOverlayDirection>(direction), id_, neighbor.id_);
  }
  void                       clear_neighbor                (const direction direction)
  {
    vr::VROverlay()->SetOverlayNeighbor(static_cast<vr::EOverlayDirection>(direction), id_, invalid_id);
  }
  void                       focus_neighbor                (const direction direction)
  {
    vr::VROverlay()->MoveGamepadFocusToNeighbor(static_cast<vr::EOverlayDirection>(direction), id_);
  }

  void                       set_raycast_mask              (const std::vector<rectangle<float>>& rectangles)
  {
    std::vector<vr::VROverlayIntersectionMaskPrimitive_t> primitives;
    for(auto& rectangle : rectangles)
      primitives.push_back({vr::OverlayIntersectionPrimitiveType_Rectangle, vr::IntersectionMaskRectangle_t{rectangle.left, rectangle.top, fabs(rectangle.right - rectangle.left), fabs(rectangle.bottom - rectangle.top)}});
    vr::VROverlay()->SetOverlayIntersectionMask(id_, primitives.data(), static_cast<std::uint32_t>(primitives.size()));
  }
  raycast_result             raycast                       (const std::array<float, 3>& position, const std::array<float, 3>& direction, tracking_mode mode = tracking_mode::automatic) const
  {
    if (mode == tracking_mode::automatic)
      mode = static_cast<tracking_mode>(vr::VRCompositor()->GetTrackingSpace());

    vr::VROverlayIntersectionParams_t  parameters
    {
      {position [0], position [1], position [2]},
      {direction[0], direction[1], direction[2]},
      static_cast<vr::ETrackingUniverseOrigin>(mode)
    };
    vr::VROverlayIntersectionResults_t results;
    vr::VROverlay()->ComputeOverlayIntersection(id_, &parameters, &results);
    return
    {
      {results.vPoint .v[0], results.vPoint .v[1], results.vPoint .v[2]},
      {results.vNormal.v[0], results.vNormal.v[1], results.vNormal.v[2]},
      {results.vUVs   .v[0], results.vUVs   .v[1]},
      results.fDistance
    };
  }

  // IVR Overlay - Texture
  void                       set_texture                   (const std::array<std::size_t, 3>&  dimensions     , const std::vector<std::uint8_t>& data)
  {
    vr::VROverlay()->SetOverlayRaw(id_, reinterpret_cast<void*>(const_cast<std::uint8_t*>(data.data())), static_cast<std::uint32_t>(dimensions[0]), static_cast<std::uint32_t>(dimensions[1]), static_cast<std::uint32_t>(dimensions[2]));
  }
  void                       set_texture                   (ID3D11Texture2D*                   texture_ptr    , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(texture_ptr), vr::TextureType_DirectX, static_cast<vr::EColorSpace>(color_space)};
    vr::VROverlay()->SetOverlayTexture(id_, &texture);
  }
  void                       set_texture                   (texture_data_d3d12                 texture_data   , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(&texture_data), vr::TextureType_DirectX12, static_cast<vr::EColorSpace>(color_space)};
    vr::VROverlay()->SetOverlayTexture(id_, &texture);
  }
  void                       set_texture                   (void*                              io_surface_ref , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {io_surface_ref, vr::TextureType_IOSurface, static_cast<vr::EColorSpace>(color_space)};
    vr::VROverlay()->SetOverlayTexture(id_, &texture);
  }
  void                       set_texture                   (std::uint32_t                      texture_id     , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(static_cast<std::uintptr_t>(texture_id)), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    vr::VROverlay()->SetOverlayTexture(id_, &texture);
  }
  void                       set_texture                   (texture_data_vulkan                texture_data   , color_space color_space = color_space::automatic)
  {
    vr::Texture_t texture {reinterpret_cast<void*>(&texture_data), vr::TextureType_OpenGL, static_cast<vr::EColorSpace>(color_space)};
    vr::VROverlay()->SetOverlayTexture(id_, &texture);
  }
  void                       clear_texture                 () const
  {
    vr::VROverlay()->ClearOverlayTexture(id_);
  }
  std::array<std::size_t, 2> texture_size                  () const
  {
    std::uint32_t width, height;
    vr::VROverlay()->GetOverlayTextureSize(id_, &width, &height);
    return {static_cast<std::size_t>(width), static_cast<std::size_t>(height)};
  }
  
  void                       from_file                     (const std::string& filepath) const
  {
    vr::VROverlay()->SetOverlayFromFile(id_, filepath.c_str());
  }

  static void                show_keyboard                 (const std::string& description = "", const std::string& text = "", const bool submit = true, const bool password = false, const bool multiline = false, const bool minimal = false, const std::size_t& max_characters = 32 * 1024, const std::uint64_t user_data = 0u)
  {
    auto          input_mode = vr::k_EGamepadTextInputModeNormal  ;
    if (submit  ) input_mode = vr::k_EGamepadTextInputModeSubmit  ;
    if (password) input_mode = vr::k_EGamepadTextInputModePassword;
    vr::VROverlay()->ShowKeyboard(
      input_mode                                                                                          ,
      multiline ? vr::k_EGamepadTextInputLineModeMultipleLines : vr::k_EGamepadTextInputLineModeSingleLine, 
      description.c_str()                                                                                 , 
      static_cast<std::uint32_t>(max_characters)                                                          , 
      text       .c_str()                                                                                 , 
      minimal                                                                                             , 
      user_data                                                                                           );
  }
  static void                hide_keyboard                 ()
  {
    vr::VROverlay()->HideKeyboard();
  }
  static void                set_keyboard_transform        (const std::array<float, 12>& matrix, tracking_mode mode = tracking_mode::automatic)
  {
    if (mode == tracking_mode::automatic)
      mode = static_cast<tracking_mode>(vr::VRCompositor()->GetTrackingSpace());
    
    vr::HmdMatrix34_t native_matrix;
    std::copy(matrix.begin(), matrix.end(), &native_matrix.m[0][0]);
    vr::VROverlay()->SetKeyboardTransformAbsolute(static_cast<vr::ETrackingUniverseOrigin>(mode), &native_matrix);
  }
  static std::string         keyboard_text                 ()
  {
    char text[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetKeyboardText(text, vr::k_unMaxPropertyStringSize);
    return std::string(text);
  }
                                                           
  void                       show_keyboard                 (const std::string& description = "", const std::string& text = "", const bool submit = true, const bool password = false, const bool multiline = false, const bool minimal = false, const std::size_t& max_characters = 32 * 1024)
  {
    auto          input_mode = vr::k_EGamepadTextInputModeNormal  ;
    if (submit  ) input_mode = vr::k_EGamepadTextInputModeSubmit  ;
    if (password) input_mode = vr::k_EGamepadTextInputModePassword;
    vr::VROverlay()->ShowKeyboardForOverlay(
      id_                                                                                                 , 
      input_mode                                                                                          ,
      multiline ? vr::k_EGamepadTextInputLineModeMultipleLines : vr::k_EGamepadTextInputLineModeSingleLine, 
      description.c_str()                                                                                 , 
      static_cast<std::uint32_t>(max_characters)                                                          , 
      text       .c_str()                                                                                 , 
      minimal                                                                                             , 
      id_                                                                                                 );
  }
  void                       set_keyboard_rectangle        (const rectangle<float>& rectangle)
  {
    const vr::HmdRect2_t native_rectangle {rectangle.left, rectangle.top, rectangle.right, rectangle.bottom};
    vr::VROverlay()->SetKeyboardPositionForOverlay(id_, native_rectangle);
  }

protected:
  explicit overlay(const std::uint64_t& handle = 0) : id_(handle)
  {

  }

  static const std::uint64_t invalid_id = std::numeric_limits<std::uint64_t>::max();

  std::uint64_t id_;
};
}

#endif
