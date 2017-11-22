#ifndef NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_HPP_

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL_joystick.h>

#include <nano_engine/systems/input/joystick_power_level.hpp>

namespace ne
{
class joystick
{
public:
  explicit joystick  (std::size_t      index) : native_(SDL_JoystickOpen(static_cast<int>(index)))
  {
    if(!native_)
      throw std::runtime_error("Failed to create SDL joystick. SDL Error: " + std::string(SDL_GetError()));
  }
  joystick           (const joystick&  that) = delete ;
  joystick           (      joystick&& temp) = default;
  ~joystick          ()
  {
    SDL_JoystickClose(native_);
  }
  joystick& operator=(const joystick&  that) = delete ;
  joystick& operator=(      joystick&& temp) = default;

  static std::size_t              joystick_count()
  {
    return SDL_NumJoysticks();
  }
  static void                     update        ()
  {
    SDL_JoystickUpdate();
  }

  std::string                  name       () const
  {
    return std::string(SDL_JoystickName(native_));
  }
  std::string                  guid       () const
  {
    char native_guid[128];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(native_), native_guid, 128);
    return std::string(native_guid);
  }
  bool                         attached   () const
  {
    return static_cast<bool>(SDL_JoystickGetAttached(native_));
  }
  joystick_power_level         power_level() const
  {
    return static_cast<joystick_power_level>(SDL_JoystickCurrentPowerLevel(native_));
  }

  SDL_Joystick* native   () const
  {
    return native_;
  }
  std::uint32_t native_id() const
  {
    return static_cast<std::uint32_t>(SDL_JoystickInstanceID(native_));
  }

private:
  SDL_Joystick* native_;
};
}

#endif
