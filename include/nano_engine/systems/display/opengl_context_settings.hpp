#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_CONTEXT_SETTINGS_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_CONTEXT_SETTINGS_HPP_

#include <cstddef>
#include <cstdint>

#include <SDL2/SDL_video.h>

#include <nano_engine/systems/display/opengl_profile.hpp>

namespace ne
{
struct opengl_context_settings
{
  std::uint32_t apply() const
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK      , static_cast<int>(profile                 ));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION     , static_cast<int>(major_version           ));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION     , static_cast<int>(minor_version           ));
                                                         
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE                  , static_cast<int>(minimum_bits_red        ));
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE                , static_cast<int>(minimum_bits_green      ));
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE                 , static_cast<int>(minimum_bits_blue       ));
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE                , static_cast<int>(minimum_bits_alpha      ));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE            , static_cast<int>(minimum_bits_accum_red  ));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE          , static_cast<int>(minimum_bits_accum_green));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE           , static_cast<int>(minimum_bits_accum_blue ));
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE          , static_cast<int>(minimum_bits_accum_alpha));
                                                         
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE               , static_cast<int>(minimum_bits_framebuffer));
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE                , static_cast<int>(minimum_bits_depth      ));
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE              , static_cast<int>(minimum_bits_stencil    ));
                                                         
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS        , static_cast<int>(msaa_buffers            ));
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES        , static_cast<int>(msaa_samples            ));

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER              , double_buffering      ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL        , hardware_acceleration ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR  , flush_on_release      ? SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH : SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, share_with_current    ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_STEREO                    , stereo                ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE  , srgb_capable          ? 1 : 0);
    
    auto flags = 0;
    if (debug             ) flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    if (forward_compatible) flags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    if (robust_access     ) flags |= SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG;
    if (reset_isolation   ) flags |= SDL_GL_CONTEXT_RESET_ISOLATION_FLAG;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags);

    return SDL_WINDOW_OPENGL;
  }

  opengl_profile profile                  = opengl_profile::core;
  std::size_t    major_version            = 4;
  std::size_t    minor_version            = 5;
                                                           
  std::size_t    minimum_bits_red         = 3;
  std::size_t    minimum_bits_green       = 3;
  std::size_t    minimum_bits_blue        = 2;
  std::size_t    minimum_bits_alpha       = 0;
  std::size_t    minimum_bits_accum_red   = 0;
  std::size_t    minimum_bits_accum_green = 0;
  std::size_t    minimum_bits_accum_blue  = 0;
  std::size_t    minimum_bits_accum_alpha = 0;
                   
  std::size_t    minimum_bits_framebuffer = 0;
  std::size_t    minimum_bits_depth       = 16;
  std::size_t    minimum_bits_stencil     = 0;
                                                           
  std::size_t    msaa_buffers             = 0;
  std::size_t    msaa_samples             = 0;
                                                           
  bool           double_buffering         = true;
  bool           hardware_acceleration    = true;
  bool           flush_on_release         = true;
  bool           share_with_current       = false;
  bool           stereo                   = false;
  bool           srgb_capable             = false;
                   
  bool           debug                    = false;
  bool           forward_compatible       = false;
  bool           robust_access            = false;
  bool           reset_isolation          = false;
};
}

#endif
