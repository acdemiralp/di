#ifndef NANO_ENGINE_UTILIY_GL_CONTEXT_HPP_
#define NANO_ENGINE_UTILIY_GL_CONTEXT_HPP_

#include <iostream>

#include <SDL2/SDL.h>

#include <nano_engine/systems/display.hpp>

namespace ne
{
class gl_context
{
public:
  enum class profile   : unsigned
  {
    core             = 0x0001,
    compatibility    = 0x0002,
    embedded_systems = 0x0004
  };
  enum class swap_mode : int
  {
    late_swap_tearing = -1,
    immediate         =  0,
    vertical_sync     =  1
  };

  gl_context(const display& display, unsigned major_version, unsigned minor_version, profile profile)
  : display_(display)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , int(profile ));
  }
  gl_context           (const gl_context&  that) = delete ;
  gl_context           (      gl_context&& temp) = default;
  virtual ~gl_context  ()
  {
    if(native_ != nullptr)
      SDL_GL_DeleteContext(native_);
  }
  gl_context& operator=(const gl_context&  that) = delete ;
  gl_context& operator=(      gl_context&& temp) = default;

  void create       ()
  {
    if (native_ != nullptr)
    {
      std::cout << "Failed to create the context. Already exists." << std::endl;
      return;
    }
    native_ = SDL_GL_CreateContext(display_.window_);
    if (native_ == nullptr)
    {
      std::cout << "Failed to create the context. SDL Error: " << SDL_GetError() << std::endl;
      return;
    }

    set_swap_mode(swap_mode::vertical_sync);
  }
  void set_swap_mode(swap_mode swap_mode) const
  {
    if (SDL_GL_SetSwapInterval(static_cast<int>(swap_mode)) < 0)
      std::cout << "Failed to set swap interval. SDL Error: " << SDL_GetError() << std::endl;
  }
  void swap_buffers ()                    const
  {
    if (display_.window_ == nullptr)
    {
      std::cout << "Failed to swap buffers. Window inexistent." << std::endl;
      return;
    }
    SDL_GL_SwapWindow(display_.window_);
  }

protected:
  const display& display_ ;
  void*          native_  = nullptr;
};
}

#endif
