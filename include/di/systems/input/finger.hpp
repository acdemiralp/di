#ifndef DI_SYSTEMS_INPUT_FINGER_HPP_
#define DI_SYSTEMS_INPUT_FINGER_HPP_

#include <array>
#include <cstddef>

namespace di
{
struct finger
{
  std::size_t          index   ;
  std::array<float, 2> position;
  float                pressure;
};
}

#endif
