#ifndef NANO_ENGINE_SYSTEMS_INPUT_KEYBOARD_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_KEYBOARD_HPP_

#include <bitset>
#include <vector>

#include <SDL2/SDL_keyboard.h>

#include <nano_engine/systems/input/key.hpp>
#include <nano_engine/systems/input/key_modifier.hpp>

namespace ne
{
namespace keyboard
{
inline std::vector<bool> key_states        ()
{
  int  key_states_size;
  auto key_states_native = SDL_GetKeyboardState(&key_states_size);
  return std::vector<bool>(key_states_native, key_states_native + key_states_size); // Reports C4800
}
inline key_modifier      modifier_state    ()
{
  return static_cast<key_modifier>(SDL_GetModState());
}
inline void              set_modifier_state(const key_modifier& state)
{
  SDL_SetModState(static_cast<SDL_Keymod>(state));
}

inline bool screen_keyboard_support()
{
  return SDL_HasScreenKeyboardSupport() != 0;
}
}
}

#endif
