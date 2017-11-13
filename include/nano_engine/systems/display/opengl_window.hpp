#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_WINDOW_HPP_

#include <cstddef>
#include <stdexcept>

#include <SDL2/SDL.h>

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
  enum class swap_mode
  {
    late_swap_tearing = -1,
    immediate         =  0,
    vertical_sync     =  1
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
    bool        share_with_current       = false;
    bool        stereo                   = false;
    bool        srgb_capable             = false;

    bool        debug                    = false;
    bool        forward_compatible       = false;
    bool        robust_access            = false;
    bool        reset_isolation          = false;
  };

  opengl_window  (
    const std::string&                title    , 
    const std::array<std::size_t, 2>& position , 
    const std::array<std::size_t, 2>& size     , 
    const context_settings&           settings = context_settings()) 
  : window(title, position, size, apply_context_settings(settings)), opengl_context_(SDL_GL_CreateContext(native_))
  {
    if (!opengl_context_)
      throw std::runtime_error("Failed to create OpenGL context. SDL Error: " + std::string(SDL_GetError()));
  }
  explicit opengl_window(
    const std::string&      title    , 
    const context_settings& settings = context_settings()) 
  : window(title, apply_context_settings(settings)), opengl_context_(SDL_GL_CreateContext(native_))
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

  bool      is_current   ()                    const
  {
    return SDL_GL_GetCurrentWindow() == native_ && SDL_GL_GetCurrentContext() == opengl_context_;
  }
  void      make_current ()                    const
  {
    SDL_GL_MakeCurrent(native_, opengl_context_);
  }
  void      set_swap_mode(swap_mode swap_mode) const
  {
    if(!is_current()) make_current();
    SDL_GL_SetSwapInterval(static_cast<int>(swap_mode));
  }
  swap_mode swap_mode    ()                    const
  {
    if (!is_current()) make_current();
    return static_cast<enum class swap_mode>(SDL_GL_GetSwapInterval());
  }
  void      swap         ()                    const
  {
    SDL_GL_SwapWindow(native_); 
  }

private:
  static unsigned apply_context_settings(const context_settings& settings = context_settings())
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK      , static_cast<int>(settings.profile));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION     , static_cast<int>(settings.major_version));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION     , static_cast<int>(settings.minor_version));
                                                         
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE                  , static_cast<int>(settings.minimum_bits_red        ));
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE                , static_cast<int>(settings.minimum_bits_green      ));
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE                 , static_cast<int>(settings.minimum_bits_blue       ));
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE                , static_cast<int>(settings.minimum_bits_alpha      ));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE            , static_cast<int>(settings.minimum_bits_accum_red  ));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE          , static_cast<int>(settings.minimum_bits_accum_green));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE           , static_cast<int>(settings.minimum_bits_accum_blue ));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE          , static_cast<int>(settings.minimum_bits_accum_alpha));
                                                         
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE               , static_cast<int>(settings.minimum_bits_framebuffer));
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE                , static_cast<int>(settings.minimum_bits_depth      ));
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE              , static_cast<int>(settings.minimum_bits_stencil    ));
                                                         
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS        , static_cast<int>(settings.msaa_buffers));
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES        , static_cast<int>(settings.msaa_samples));

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER              , settings.double_buffering      ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL        , settings.hardware_acceleration ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR  , settings.flush_on_release      ? SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH : SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, settings.share_with_current    ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_STEREO                    , settings.stereo                ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE  , settings.srgb_capable          ? 1 : 0);
    
    auto context_flags = 0;
    if (settings.debug             ) context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    if (settings.forward_compatible) context_flags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    if (settings.robust_access     ) context_flags |= SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG;
    if (settings.reset_isolation   ) context_flags |= SDL_GL_CONTEXT_RESET_ISOLATION_FLAG;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);

    return SDL_WINDOW_OPENGL;
  }
  
  SDL_GLContext opengl_context_ = nullptr;
};
}

#endif
