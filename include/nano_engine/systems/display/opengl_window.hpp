#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_WINDOW_HPP_

#include <nano_engine/systems/display/window.hpp>

namespace ne
{
class opengl_window final : public window
{
public:
  enum class profile
  {
    core             = SDL_GL_CONTEXT_PROFILE_CORE         ,
    compatibility    = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
    embedded_systems = SDL_GL_CONTEXT_PROFILE_ES
  };

  struct context_settings
  {
    profile     profile                  = profile::core;
    std::size_t major_version            = 4;
    std::size_t minor_version            = 5;
                                                        
    std::size_t minimum_bits_red         = 3;
    std::size_t minimum_bits_green       = 3;
    std::size_t minimum_bits_blue        = 2;
    std::size_t minimum_bits_alpha       = 0;
    std::size_t minimum_bits_accum_red   = 0;
    std::size_t minimum_bits_accum_green = 0;
    std::size_t minimum_bits_accum_blue  = 0;
    std::size_t minimum_bits_accum_alpha = 0;

    std::size_t minimum_bits_framebuffer = 0;
    std::size_t minimum_bits_depth       = 16;
    std::size_t minimum_bits_stencil     = 0;
                                                        
    std::size_t msaa_buffers             = 0;
    std::size_t msaa_samples             = 0;
                                                        
    bool        double_buffering         = true;
    bool        hardware_acceleration    = true;
    bool        flush_on_release         = true;
    bool        stereo                   = false;
    bool        share_with_current       = false;
    bool        srgb_capable             = false;

    bool        debug                    = false;
    bool        forward_compatible       = false;
    bool        robust_access            = false;
    bool        reset_isolation          = false;
  };

  opengl_window           (const context_settings& settings = context_settings()) : window(SDL_WINDOW_OPENGL), opengl_context_(SDL_GL_CreateContext(native_))
  {
    if (!opengl_context_)
      throw std::runtime_error("Failed to create OpenGL context. SDL Error: " + std::string(SDL_GetError()));
  }
  opengl_window           (const opengl_window&  that) = delete ;
  opengl_window           (      opengl_window&& temp) = default;
  ~opengl_window  ()
  {
    SDL_GL_DeleteContext(opengl_context_);
  }
  opengl_window& operator=(const opengl_window&  that) = delete ;
  opengl_window& operator=(      opengl_window&& temp) = default;

  void swap() const
  {
    SDL_GL_SwapWindow(native_); 
  }

protected:
  SDL_GLContext opengl_context_ = nullptr;
};
}

#endif
