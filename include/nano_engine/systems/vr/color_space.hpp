#ifndef NANO_ENGINE_SYSTEMS_VR_COLOR_SPACE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_COLOR_SPACE_HPP_

#include <openvr.h>

namespace ne
{
enum class color_space
{
  automatic = vr::ColorSpace_Auto  ,
  gamma     = vr::ColorSpace_Gamma ,
  linear    = vr::ColorSpace_Linear
};
}

#endif