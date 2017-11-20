#ifndef NANO_ENGINE_SYSTEMS_INPUT_KEY_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_KEY_HPP_

#include <string>

#include <SDL2/SDL_keyboard.h>

#include <nano_engine/systems/input/key_code.hpp>
#include <nano_engine/systems/input/key_modifier.hpp>
#include <nano_engine/systems/input/scan_code.hpp>

namespace ne
{
struct key
{
  key_code    key_code() const
  {
    return static_cast<ne::key_code>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(scan_code)));
  }
  std::string name    () const
  {
    return std::string(SDL_GetScancodeName(static_cast<SDL_Scancode>(scan_code)));
  }

  scan_code    scan_code;
  key_modifier modifiers;
};
}

#endif
