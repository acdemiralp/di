#ifndef DI_SYSTEMS_VR_RAYCAST_RESULT_HPP_
#define DI_SYSTEMS_VR_RAYCAST_RESULT_HPP_

#include <array>

namespace di
{
struct raycast_result
{
  std::array<float, 3> position           ;
  std::array<float, 3> normal             ;
  std::array<float, 2> texture_coordinates;
  float                distance           ;
};
}

#endif