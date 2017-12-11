#ifndef NANO_ENGINE_SYSTEMS_INPUT_HAPTIC_DEVICE_INFO_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_HAPTIC_DEVICE_INFO_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include <SDL2/SDL_haptic.h>

namespace ne
{
struct haptic_device_info
{
  std::size_t index;
  std::string name ;
  bool        open ;
};
  
inline std::vector<haptic_device_info> haptic_device_infos()
{
  std::vector<haptic_device_info> haptic_device_infos(SDL_NumHaptics());
  for (auto i = 0; i < haptic_device_infos.size(); ++i)
    haptic_device_infos[i] = {static_cast<std::size_t>(i), std::string(SDL_HapticName(i)), SDL_HapticOpened(i) != 0};
  return haptic_device_infos;
}
}

#endif
