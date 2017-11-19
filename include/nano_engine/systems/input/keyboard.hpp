#ifndef NANO_ENGINE_SYSTEMS_INPUT_KEYBOARD_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_KEYBOARD_HPP_

#include <bitset>
#include <vector>

#include <SDL2/SDL_keyboard.h>

namespace ne
{
namespace keyboard
{
inline std::vector<bool> key_states    ()
{
  int  key_states_size;
  auto key_states_native = SDL_GetKeyboardState(&key_states_size);
  return std::vector<bool>(key_states_native, key_states_native + key_states_size); // Reports C4800
}
inline std::bitset<32>   mod_states    ()
{
  return SDL_GetModState();
}
inline void              set_mod_states(std::bitset<32> state)
{
  SDL_SetModState(SDL_Keymod(static_cast<int>(state.to_ulong())));
}

inline bool screen_keyboard_support()
{
  return SDL_HasScreenKeyboardSupport() != 0;
}
}
}

#endif
