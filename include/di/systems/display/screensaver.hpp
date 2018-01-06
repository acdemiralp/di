#ifndef DI_SYSTEMS_DISPLAY_SCREENSAVER_HPP_
#define DI_SYSTEMS_DISPLAY_SCREENSAVER_HPP_

#include <SDL2/SDL_video.h>

namespace di
{
namespace screensaver
{
inline void set_enabled(const bool enabled)
{
  enabled ? SDL_EnableScreenSaver() : SDL_DisableScreenSaver();
}
inline bool enabled    ()
{
  return SDL_IsScreenSaverEnabled() != 0;
}
}
}

#endif
