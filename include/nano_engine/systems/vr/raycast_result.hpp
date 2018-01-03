#ifndef NANO_ENGINE_SYSTEMS_VR_RAYCAST_RESULT_HPP_
#define NANO_ENGINE_SYSTEMS_VR_RAYCAST_RESULT_HPP_

#include <array>

namespace ne
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