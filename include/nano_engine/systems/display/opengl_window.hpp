#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_WINDOW_HPP_

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

#include <SDL2/SDL_video.h>

#include <nano_engine/systems/display/opengl_context_settings.hpp>
#include <nano_engine/systems/display/opengl_swap_mode.hpp>
#include <nano_engine/systems/display/window.hpp>

namespace ne
{
class opengl_window final : public window
{
public:
  explicit opengl_window  (const std::string& title, const opengl_context_settings& settings = opengl_context_settings()) 
  : window(title, settings.apply()), opengl_context_(SDL_GL_CreateContext(native_))
  {
    if (!opengl_context_)
      throw std::runtime_error("Failed to create OpenGL context. SDL Error: " + std::string(SDL_GetError())); 
  }
  opengl_window           (const std::string& title, const std::array<std::size_t, 2>& position, const std::array<std::size_t, 2>& size, const opengl_context_settings& settings = opengl_context_settings()) 
  : window(title, position, size, settings.apply()), opengl_context_(SDL_GL_CreateContext(native_))
  {
    if (!opengl_context_)
      throw std::runtime_error("Failed to create OpenGL context. SDL Error: " + std::string(SDL_GetError()));
  }
  opengl_window           (const opengl_window&  that) = delete ;
  opengl_window           (      opengl_window&& temp) = default;
  ~opengl_window          ()
  {
    SDL_GL_DeleteContext(opengl_context_);
  }
  opengl_window& operator=(const opengl_window&  that) = delete ;
  opengl_window& operator=(      opengl_window&& temp) = default;

  void update() override
  {
    SDL_GL_SwapWindow(native_);
  }

  bool                       current      ()                           const
  {
    return SDL_GL_GetCurrentWindow() == native_ && SDL_GL_GetCurrentContext() == opengl_context_;
  }
  void                       set_current  ()                           const
  {
    SDL_GL_MakeCurrent(native_, opengl_context_);
  }
  opengl_swap_mode           swap_mode    ()                           const
  {
    if (!current()) set_current();
    return static_cast<opengl_swap_mode>(SDL_GL_GetSwapInterval());
  }
  void                       set_swap_mode(opengl_swap_mode swap_mode) const
  {
    if (!current()) set_current();
    SDL_GL_SetSwapInterval(static_cast<int>(swap_mode));
  }
  std::array<std::size_t, 2> drawable_size()                           const
  {
    std::array<std::size_t, 2> drawable_size;
    SDL_GL_GetDrawableSize(native_, reinterpret_cast<int*>(&drawable_size[0]), reinterpret_cast<int*>(&drawable_size[1]));
    return drawable_size;
  }

private:
  SDL_GLContext opengl_context_ = nullptr;
};
}

#endif
