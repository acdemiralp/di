#ifndef NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include <boost/optional.hpp>
#include <openvr.h>

#include <nano_engine/systems/vr/color_space.hpp>
#include <nano_engine/utility/rectangle.hpp>

#undef min
#undef max

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
  
  std::string                key                    ()                                                                                                          const
  {
    char key[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayKey(id_, key, vr::k_unMaxPropertyStringSize);
    return std::string(key);
  }
  std::array<std::size_t, 2> image_size             ()                                                                                                          const
  {
    std::array<std::uint32_t, 2> size;
    vr::VROverlay()->GetOverlayImageData(id_, nullptr, 0, &size[0], &size[1]);
    return {static_cast<std::size_t>(size[0]), static_cast<std::size_t>(size[1])};
  }
  std::vector<std::uint8_t>  image_data             ()                                                                                                          const
  {
    auto size = image_size();
    std::vector<std::uint8_t> data(4 * size[0] * size[1]);
    vr::VROverlay()->GetOverlayImageData(id_, data.data(), data.size(), nullptr, nullptr);
    return data;
  }
  bool                       is_hover_target_display()                                                                                                          const
  {
    return vr::VROverlay()->IsHoverTargetOverlay(id_);
  }
  
  void                       set_name               (const std::string&          name              )        
  {
    vr::VROverlay()->SetOverlayName(id_, name.c_str());
  }  
  void                       set_visible            (const bool                  visible           ) 
  {
    visible ? vr::VROverlay()->ShowOverlay(id_) : vr::VROverlay()->HideOverlay(id_);
  }       
  void                       set_color              (const std::array<float, 4>& color             )
  {
    vr::VROverlay()->SetOverlayColor(id_, color[0], color[1], color[2]);
    vr::VROverlay()->SetOverlayAlpha(id_, color[2]);
  }
  void                       set_color_space        (const color_space           color_space       )
  {
    vr::VROverlay()->SetOverlayTextureColorSpace(id_, static_cast<vr::EColorSpace>(color_space));
  }
  void                       set_texture_bounds     (const rectangle<float>&     texture_bounds    )
  {
    vr::VRTextureBounds_t bounds {texture_bounds.left, texture_bounds.bottom, texture_bounds.right, texture_bounds.top};
    vr::VROverlay()->SetOverlayTextureBounds(id_, &bounds);
  }
  void                       set_width              (const float                 width             ) 
  {
    vr::VROverlay()->SetOverlayWidthInMeters(id_, width);
  }
  void                       set_texel_aspect_ratio (const float                 texel_aspect_ratio) 
  {
    vr::VROverlay()->SetOverlayTexelAspect(id_, texel_aspect_ratio);
  }
  void                       set_sort_order         (const std::uint32_t         sort_order        ) 
  {
    vr::VROverlay()->SetOverlaySortOrder(id_, sort_order);
  }
  void                       set_model              (const std::string&          model_name        , boost::optional<std::array<float, 4>> color = boost::none)
  {
    vr::HmdColor_t native_color;
    if (color != boost::none)
      native_color = {color->at(0), color->at(1), color->at(2), color->at(3)};
    vr::VROverlay()->SetOverlayRenderModel(id_, model_name.c_str(), color != boost::none ? &native_color : nullptr);
  }
  void                       set_rendering_pid      (const std::uint32_t         rendering_pid     )        
  {
    vr::VROverlay()->SetOverlayRenderingPid(id_, rendering_pid);
  }  
  void                       set_high_quality       ()
  {
    vr::VROverlay()->SetHighQualityOverlay(id_);
  }                                      
  void                       set_gamepad_focus      ()
  {
    vr::VROverlay()->SetGamepadFocusOverlay(id_);
  }                                      
  void                       set_mouse_scale        (const std::array<float, 2>& mouse_scale       ) 
  {
    vr::HmdVector2_t scale {{mouse_scale[0], mouse_scale[1]}};
    vr::VROverlay()->SetOverlayMouseScale(id_, &scale);
  }
  void                       set_auto_curve_range   (const std::array<float, 2>& auto_curve_range  ) 
  {
    vr::VROverlay()->SetOverlayAutoCurveDistanceRangeInMeters(id_, auto_curve_range[0], auto_curve_range[1]);
  }
  
  std::string                name                   ()                                                                                                          const
  {
    char name[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayName(id_, name, vr::k_unMaxPropertyStringSize);
    return std::string(name);
  }                                            
  bool                       visible                ()                                                                                                          const
  {
    return vr::VROverlay()->IsOverlayVisible(id_);
  }
  std::array<float, 4>       color                  ()                                                                                                          const
  {
    std::array<float, 4> color;
    vr::VROverlay()->GetOverlayColor(id_, &color[0], &color[1], &color[2]);
    vr::VROverlay()->GetOverlayAlpha(id_, &color[3]);
    return color;
  }
  color_space                color_space            ()                                                                                                          const
  {
    vr::EColorSpace color_space;
    vr::VROverlay()->GetOverlayTextureColorSpace(id_, &color_space);
    return static_cast<ne::color_space>(color_space);
  }
  rectangle<float>           texture_bounds         ()                                                                                                          const
  {
    vr::VRTextureBounds_t texture_bounds;
    vr::VROverlay()->SetOverlayTextureBounds(id_, &texture_bounds);
    return rectangle<float> {texture_bounds.uMin, texture_bounds.vMin, texture_bounds.uMax, texture_bounds.vMax};
  }
  float                      width                  ()                                                                                                          const
  {
    float  width;
    vr::VROverlay()->GetOverlayWidthInMeters(id_, &width);
    return width;
  }  
  float                      texel_aspect_ratio     ()                                                                                                          const
  {
    float  texel_aspect_ratio;
    vr::VROverlay()->GetOverlayTexelAspect(id_, &texel_aspect_ratio);
    return texel_aspect_ratio;
  }   
  std::uint32_t              sort_order             ()                                                                                                          const
  {
    std::uint32_t sort_order;
    vr::VROverlay()->GetOverlaySortOrder(id_, &sort_order);
    return        sort_order;
  }                                                               
  std::string                render_model_name      ()                                                                                                          const
  {
    char                render_model_name[vr::k_unMaxPropertyStringSize];
    vr::HmdColor_t      color;
    vr::EVROverlayError error;
    vr::VROverlay()->GetOverlayRenderModel(id_, render_model_name, vr::k_unMaxPropertyStringSize, &color, &error);
    return std::string(render_model_name);
  }
  std::array<float, 4>       render_model_color     ()                                                                                                          const
  {
    char                render_model_name[vr::k_unMaxPropertyStringSize];
    vr::HmdColor_t      color;
    vr::EVROverlayError error;
    vr::VROverlay()->GetOverlayRenderModel(id_, render_model_name, vr::k_unMaxPropertyStringSize, &color, &error);
    return {color.r, color.g, color.b, color.a};
  }
  std::uint32_t              rendering_pid          ()                                                                                                          const
  {
    return vr::VROverlay()->GetOverlayRenderingPid(id_);
  } 
  bool                       is_high_quality        ()                                                                                                          const
  {
    return vr::VROverlay()->GetHighQualityOverlay() == id_;
  }
  bool                       gamepad_focus          ()                                                                                                          const
  {
    return vr::VROverlay()->GetGamepadFocusOverlay() == id_;
  }
  std::array<float, 2>       mouse_scale            ()                                                                                                          const
  {
    vr::HmdVector2_t mouse_scale;
    vr::VROverlay()->GetOverlayMouseScale(id_, &mouse_scale);
    return {mouse_scale.v[0], mouse_scale.v[1]};
  } 
  std::array<float, 2>       auto_curve_range       ()                                                                                                          const
  {
    std::array<float, 2> auto_curve_range;
    vr::VROverlay()->GetOverlayAutoCurveDistanceRangeInMeters(id_, &auto_curve_range[0], &auto_curve_range[1]);
    return               auto_curve_range;
  } 

  static overlay             find                   (const std::string& key)
  {
    std::uint64_t handle;
    vr::VROverlay()->FindOverlay(key.c_str(), &handle);
    return overlay(handle);
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
