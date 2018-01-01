#ifndef NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <openvr.h>

#include <nano_engine/systems/vr/color_space.hpp>
#include <nano_engine/utility/rectangle.hpp>

namespace ne
{
class overlay
{
public:
  explicit overlay  (const std::string& key, const std::string& name)
  {
    vr::VROverlay()->CreateOverlay(key.c_str(), name.c_str(), &handle_);
  }
  overlay           (const overlay&  that) = default;
  overlay           (      overlay&& temp) = default;
  virtual ~overlay  ()
  {
    vr::VROverlay()->DestroyOverlay(handle_);
  }
  overlay& operator=(const overlay&  that) = default;
  overlay& operator=(      overlay&& temp) = default;
  
  std::string                key                    ()                                               const
  {
    char key[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayKey(handle_, key, vr::k_unMaxPropertyStringSize);
    return std::string(key);
  }
  std::array<std::size_t, 2> image_size             ()                                               const
  {
    std::array<std::uint32_t, 2> size;
    vr::VROverlay()->GetOverlayImageData(handle_, nullptr, 0, &size[0], &size[1]);
    return {static_cast<std::size_t>(size[0]), static_cast<std::size_t>(size[1])};
  }
  std::vector<std::uint8_t>  image_data             ()                                               const
  {
    auto size = image_size();
    std::vector<std::uint8_t> data(4 * size[0] * size[1]);
    vr::VROverlay()->GetOverlayImageData(handle_, data.data(), data.size(), nullptr, nullptr);
    return data;
  }
  bool                       is_hover_target_display()                                               const
  {
    return vr::VROverlay()->IsHoverTargetOverlay(handle_);
  }
  
  void                       set_name               (const std::string&          name              )        
  {
    vr::VROverlay()->SetOverlayName(handle_, name.c_str());
  }  
  void                       set_visible            (const bool                  visible           ) 
  {
    visible ? vr::VROverlay()->ShowOverlay(handle_) : vr::VROverlay()->HideOverlay(handle_);
  }       
  void                       set_color              (const std::array<float, 4>& color             )
  {
    vr::VROverlay()->SetOverlayColor(handle_, color[0], color[1], color[2]);
    vr::VROverlay()->SetOverlayAlpha(handle_, color[2]);
  }
  void                       set_color_space        (const color_space           color_space       )
  {
    vr::VROverlay()->SetOverlayTextureColorSpace(handle_, static_cast<vr::EColorSpace>(color_space));
  }
  void                       set_texture_bounds     (const rectangle<float>&     texture_bounds    )
  {
    vr::VRTextureBounds_t bounds {texture_bounds.left, texture_bounds.bottom, texture_bounds.right, texture_bounds.top};
    vr::VROverlay()->SetOverlayTextureBounds(handle_, &bounds);
  }
  void                       set_width              (const float                 width             ) 
  {
    vr::VROverlay()->SetOverlayWidthInMeters(handle_, width);
  }
  void                       set_texel_aspect_ratio (const float                 texel_aspect_ratio) 
  {
    vr::VROverlay()->SetOverlayTexelAspect(handle_, texel_aspect_ratio);
  }
  void                       set_sort_order         (const std::uint32_t         sort_order        ) 
  {
    vr::VROverlay()->SetOverlaySortOrder(handle_, sort_order);
  }
  void                       set_rendering_pid      (const std::uint32_t         rendering_pid     )        
  {
    vr::VROverlay()->SetOverlayRenderingPid(handle_, rendering_pid);
  }  
  void                       set_high_quality       ()
  {
    vr::VROverlay()->SetHighQualityOverlay(handle_);
  }                                      
  void                       set_gamepad_focus      ()
  {
    vr::VROverlay()->SetGamepadFocusOverlay(handle_);
  }                                      
  void                       set_mouse_scale        (const std::array<float, 2>& mouse_scale       ) 
  {
    vr::HmdVector2_t native_scale {{mouse_scale[0], mouse_scale[1]}};
    vr::VROverlay()->SetOverlayMouseScale(handle_, &native_scale);
  }
  void                       set_auto_curve_range   (const std::array<float, 2>& auto_curve_range  ) 
  {
    vr::VROverlay()->SetOverlayAutoCurveDistanceRangeInMeters(handle_, auto_curve_range[0], auto_curve_range[1]);
  }
                                                                                                     
  std::string                name                   ()                                               const
  {
    char name[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayName(handle_, name, vr::k_unMaxPropertyStringSize);
    return std::string(name);
  }                                            
  bool                       visible                ()                                               const
  {
    return vr::VROverlay()->IsOverlayVisible(handle_);
  }
  std::array<float, 4>       color                  ()                                               const
  {
    std::array<float, 4> color;
    vr::VROverlay()->GetOverlayColor(handle_, &color[0], &color[1], &color[2]);
    vr::VROverlay()->GetOverlayAlpha(handle_, &color[3]);
    return color;
  }
  color_space                color_space            ()                                               const
  {
    vr::EColorSpace color_space;
    vr::VROverlay()->GetOverlayTextureColorSpace(handle_, &color_space);
    return static_cast<ne::color_space>(color_space);
  }
  rectangle<float>           texture_bounds         ()                                               const
  {
    vr::VRTextureBounds_t bounds;
    vr::VROverlay()->SetOverlayTextureBounds(handle_, &bounds);
    return rectangle<float> {bounds.uMin, bounds.vMin, bounds.uMax, bounds.vMax};
  }
  float                      width                  ()                                               const
  {
    float  width;
    vr::VROverlay()->GetOverlayWidthInMeters(handle_, &width);
    return width;
  }  
  float                      texel_aspect_ratio     ()                                               const
  {
    float  ratio;
    vr::VROverlay()->GetOverlayTexelAspect(handle_, &ratio);
    return ratio;
  }   
  std::uint32_t              sort_order             ()                                               const
  {
    std::uint32_t order;
    vr::VROverlay()->GetOverlaySortOrder(handle_, &order);
    return        order;
  }                                                               
  std::uint32_t              rendering_pid          ()                                               const
  {
    return vr::VROverlay()->GetOverlayRenderingPid(handle_);
  } 
  bool                       is_high_quality        ()                                               const
  {
    return vr::VROverlay()->GetHighQualityOverlay() == handle_;
  }
  bool                       gamepad_focus          ()                                               const
  {
    return vr::VROverlay()->GetGamepadFocusOverlay() == handle_;
  }
  std::array<float, 2>       mouse_scale            ()                                               const
  {
    vr::HmdVector2_t scale;
    vr::VROverlay()->GetOverlayMouseScale(handle_, &scale);
    return {scale.v[0], scale.v[1]};
  } 
  std::array<float, 2>       auto_curve_range       ()                                               const
  {
    std::array<float, 2> range;
    vr::VROverlay()->GetOverlayAutoCurveDistanceRangeInMeters(handle_, &range[0], &range[1]);
    return               range;
  } 

  static overlay             find                   (const std::string& key)
  {
    std::uint64_t handle;
    vr::VROverlay()->FindOverlay(key.c_str(), &handle);
    return overlay(handle);
  }

protected:
  explicit overlay(const std::uint64_t& handle = 0) : handle_(handle)
  {

  }
  
  std::uint64_t handle_;
};
}

#endif
