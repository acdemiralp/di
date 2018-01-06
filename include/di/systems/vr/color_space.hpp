#ifndef DI_SYSTEMS_VR_COLOR_SPACE_HPP_
#define DI_SYSTEMS_VR_COLOR_SPACE_HPP_

#include <openvr.h>

namespace di
{
enum class color_space
{
  automatic = vr::ColorSpace_Auto  ,
  gamma     = vr::ColorSpace_Gamma ,
  linear    = vr::ColorSpace_Linear
};
}

#endif