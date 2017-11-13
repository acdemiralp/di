#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_SCREENSAVER_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_SCREENSAVER_HPP_

#include <SDL2/SDL_video.h>

namespace ne
{
namespace screensaver
{
static void set_enabled(bool enabled)
{
  enabled ? SDL_EnableScreenSaver() : SDL_DisableScreenSaver();
}
static bool enabled    ()
{
  return SDL_IsScreenSaverEnabled() != 0;
}
}
}

#endif
