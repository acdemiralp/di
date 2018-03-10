#ifndef DI_SYSTEMS_DISPLAY_OPENGL_SWAP_MODE_HPP_
#define DI_SYSTEMS_DISPLAY_OPENGL_SWAP_MODE_HPP_

namespace di
{
enum class opengl_swap_mode
{
  late_swap_tearing = -1,
  immediate         =  0,
  vertical_sync     =  1
};
}

#endif
