#ifndef NANO_ENGINE_SYSTEMS_VR_CHAPERONE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_CHAPERONE_HPP_

#include <array>

#include <openvr.h>

#include <nano_engine/systems/vr/chaperone_calibration_state.hpp>
#include <nano_engine/utility/rectangle.hpp>

namespace ne
{
class chaperone
{
public:
  // IVR Chaperone
  chaperone_calibration_state      calibration_state  ()                                  const
  {
    return static_cast<chaperone_calibration_state>(vr::VRChaperone()->GetCalibrationState());
  }
  std::array<float, 2>             play_area_size     ()                                  const
  {
    std::array<float, 2> size;
    vr::VRChaperone()->GetPlayAreaSize(&size[0], &size[1]);
    return size;
  }
  rectangle <std::array<float, 2>> play_area_rectangle()                                  const
  {
    vr::HmdQuad_t rectangle;
    vr::VRChaperone()->GetPlayAreaRect(&rectangle);
    return ne::rectangle<std::array<float, 2>> 
    {
      {rectangle.vCorners[0].v[0], rectangle.vCorners[0].v[2]},
      {rectangle.vCorners[1].v[0], rectangle.vCorners[1].v[2]},
      {rectangle.vCorners[2].v[0], rectangle.vCorners[2].v[2]},
      {rectangle.vCorners[3].v[0], rectangle.vCorners[3].v[2]}
    };
  }
                                                                                     
  std::array<float, 4>             color              ()                                  const
  {
    vr::HmdColor_t color, output_camera_color;
    vr::VRChaperone()->GetBoundsColor(&color, 1, 0.0F, &output_camera_color);
    return std::array<float, 4>{color.r, color.g, color.b, color.a};
  }
  void                             set_color          (const std::array<float, 4>& color)
  {
    vr::VRChaperone()->SetSceneColor(vr::HmdColor_t{color[0], color[1], color[2], color[3]});
  }
                                   
  bool                             visible            ()                                  const
  {
    return vr::VRChaperone()->AreBoundsVisible();
  }
  void                             set_visible        (bool visible)
  {
    vr::VRChaperone()->ForceBoundsVisible(visible);
  }
                                                                                      
  void                             reload             ()                                  const
  {
    vr::VRChaperone()->ReloadInfo();
  }
};
}

#endif
