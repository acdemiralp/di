#ifndef DI_SYSTEMS_DISPLAY_DISPLAY_INFO_HPP_
#define DI_SYSTEMS_DISPLAY_DISPLAY_INFO_HPP_

#include <string>
#include <vector>

#include <SDL2/SDL_video.h>

namespace di
{
namespace video_driver
{
inline std::string              get_current()
{
  return std::string(SDL_GetCurrentVideoDriver());
}
inline std::vector<std::string> get_all    ()
{
  std::vector<std::string> drivers;
  for(auto i = 0; i < SDL_GetNumVideoDrivers(); ++i)
    drivers.emplace_back(SDL_GetVideoDriver(i));
  return drivers;
}
}
}

#endif
