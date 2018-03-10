#ifndef DI_SYSTEMS_VR_EYE_HPP_
#define DI_SYSTEMS_VR_EYE_HPP_

#include <openvr.h>

namespace di
{
enum class eye
{
  left  = vr::Eye_Left,
  right = vr::Eye_Right
};
}

#endif