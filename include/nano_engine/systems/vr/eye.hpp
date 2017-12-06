#ifndef NANO_ENGINE_SYSTEMS_VR_EYE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_EYE_HPP_

#include <openvr.h>

namespace ne
{
enum class eye
{
  left  = vr::Eye_Left,
  right = vr::Eye_Right
};
}

#endif