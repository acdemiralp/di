#ifndef NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_INFO_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_INFO_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <SDL2/SDL_joystick.h>

#include <nano_engine/systems/input/joystick_type.hpp>

namespace ne
{
struct joystick_info
{
  std::size_t   index          ;
  joystick_type type           ;
  std::string   name           ;
  std::string   uuid           ;
  std::uint32_t instance_id    ;
  std::uint32_t product_id     ;
  std::uint32_t product_version;
  std::uint32_t vendor         ;
};

inline std::vector<joystick_info> joystick_infos()
{
  std::vector<joystick_info> joystick_infos(SDL_NumJoysticks());
  for (auto i = 0; i < joystick_infos.size(); ++i)
  {
    char uuid[128];
    SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), uuid, 128);
    joystick_infos[i] = 
    {
      static_cast<std::size_t>  (i), 
      static_cast<joystick_type>(SDL_JoystickGetDeviceType          (i)),
      std::string               (SDL_JoystickNameForIndex           (i)), 
      std::string               (uuid), 
      static_cast<std::uint32_t>(SDL_JoystickGetDeviceInstanceID    (i)),
      static_cast<std::uint32_t>(SDL_JoystickGetDeviceProduct       (i)),
      static_cast<std::uint32_t>(SDL_JoystickGetDeviceProductVersion(i)),
      static_cast<std::uint32_t>(SDL_JoystickGetDeviceVendor        (i))
    };
  }
  return joystick_infos;
}
}

#endif
