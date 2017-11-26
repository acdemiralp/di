#ifndef NANO_ENGINE_SYSTEMS_INPUT_MULTI_GESTURE_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_MULTI_GESTURE_HPP_

#include <array>
#include <cstddef>

namespace ne
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
