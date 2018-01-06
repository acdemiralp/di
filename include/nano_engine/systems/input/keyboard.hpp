#ifndef NANO_ENGINE_SYSTEMS_INPUT_KEYBOARD_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_KEYBOARD_HPP_

#include <map>

#include <SDL2/SDL_keyboard.h>

#include <nano_engine/systems/input/key.hpp>
#include <nano_engine/systems/input/key_modifier.hpp>

namespace ne
{
namespace keyboard
{
inline key_modifier        modifier_state         ()
{
  return static_cast<key_modifier>(SDL_GetModState());
}
inline void                set_modifier_state     (const key_modifier& modifier)
{
  SDL_SetModState(static_cast<SDL_Keymod>(modifier));
}
inline std::map<key, bool> key_states             ()
{
  std::map<key, bool> states;
  int  key_states_size;
  const auto key_states_native = SDL_GetKeyboardState(&key_states_size);
  const auto modifier          = modifier_state();
  for(auto i = 0; i < key_states_size; ++i)
    states[key{static_cast<key_code>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(i))), modifier, static_cast<scan_code>(i)}] = static_cast<bool>(key_states_native[i]);
  return states;
}

inline bool                screen_keyboard_support()
{
  return SDL_HasScreenKeyboardSupport() != 0;
}
}
}

#endif
