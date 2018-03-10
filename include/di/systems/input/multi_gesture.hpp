#ifndef DI_SYSTEMS_INPUT_MULTI_GESTURE_HPP_
#define DI_SYSTEMS_INPUT_MULTI_GESTURE_HPP_

#include <array>
#include <cstddef>

namespace di
{
struct multi_gesture
{
  std::array<float, 2> position    ;
  float                rotation    ;
  float                scale       ;
  std::size_t          finger_count;
};

}

#endif
