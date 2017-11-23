#ifndef NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_INFO_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_INFO_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include <SDL2/SDL_joystick.h>

namespace ne
{
struct joystick_info
{
  std::size_t index;
  std::string name ;
  std::string guid ;
};

inline std::vector<joystick_info> joystick_infos()
{
  std::vector<joystick_info> joystick_infos(SDL_NumJoysticks());
  for (auto i = 0; i < joystick_infos.size(); ++i)
  {
    char guid[128];
    SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(static_cast<int>(i)), guid, 128);
    joystick_infos[i] = {i, std::string(SDL_JoystickNameForIndex(static_cast<int>(i))), std::string(guid)};
  }
  return joystick_infos;
}
}

#endif
