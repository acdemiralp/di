#ifndef NANO_ENGINE_UTILITY_OPENGL_SETTINGS_HPP_
#define NANO_ENGINE_UTILITY_OPENGL_SETTINGS_HPP_

namespace ne
{
struct opengl_settings
{
  enum class profile : unsigned
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

  unsigned  major_version = 4;
  unsigned  minor_version = 5;
  profile   profile       = profile::core;
  swap_mode swap_mode     = swap_mode::vertical_sync;
};
}

#endif
