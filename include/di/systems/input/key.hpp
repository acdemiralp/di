#ifndef DI_SYSTEMS_INPUT_KEY_HPP_
#define DI_SYSTEMS_INPUT_KEY_HPP_

#include <string>

#include <SDL2/SDL_keyboard.h>

#include <di/systems/input/key_code.hpp>
#include <di/systems/input/key_modifier.hpp>
#include <di/systems/input/scan_code.hpp>

namespace di
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
