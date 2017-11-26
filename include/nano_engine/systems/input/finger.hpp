#ifndef NANO_ENGINE_SYSTEMS_INPUT_FINGER_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_FINGER_HPP_

#include <array>
#include <cstddef>

namespace ne
{
struct finger
{
  std::size_t          index   ;
  std::array<float, 2> position;
  float                pressure;
};
}

#endif
