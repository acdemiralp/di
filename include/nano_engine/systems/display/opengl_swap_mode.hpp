#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_SWAP_MODE_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_OPENGL_SWAP_MODE_HPP_

namespace ne
{
enum class opengl_swap_mode
{
  late_swap_tearing = -1,
  immediate         =  0,
  vertical_sync     =  1
};
}

#endif
