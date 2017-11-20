#ifndef NANO_ENGINE_SYSTEMS_INPUT_KEY_MODIFIER_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_KEY_MODIFIER_HPP_

#include <nano_engine/utility/bitset_enum.hpp>

namespace ne
{
enum class key_modifier
{
  none        = 0x0000,
  left_shift  = 0x0001,
  right_shift = 0x0002,
  left_ctrl   = 0x0040,
  right_ctrl  = 0x0080,
  left_alt    = 0x0100,
  right_alt   = 0x0200,
  left_gui    = 0x0400,
  right_gui   = 0x0800,
  num_lock    = 0x1000,
  caps_lock   = 0x2000,
  mode        = 0x4000,
  reserved    = 0x8000
};

template<>
struct is_bitset_enum<key_modifier>
{
  static const bool enable = true;
};
}

#endif
