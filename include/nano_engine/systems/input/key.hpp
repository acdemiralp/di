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
  std::string name          () const
  {
    return std::string(SDL_GetKeyName     (static_cast<SDL_Keycode>(code      )));
  }
  std::string scan_code_name() const
  {
    return std::string(SDL_GetScancodeName(static_cast<SDL_Scancode>(scan_code)));
  }

  key_code     code     ;
  key_modifier modifier ;
  scan_code    scan_code;
};
}

#endif
