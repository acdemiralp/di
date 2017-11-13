#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_PROFILE_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_PROFILE_HPP_

#include <SDL2/SDL_video.h>

namespace ne
{
enum class opengl_profile
{
  core             = SDL_GL_CONTEXT_PROFILE_CORE         ,
  compatibility    = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
  embedded_systems = SDL_GL_CONTEXT_PROFILE_ES
};
}

#endif
