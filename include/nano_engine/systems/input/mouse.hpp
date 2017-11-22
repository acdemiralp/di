#ifndef NANO_ENGINE_SYSTEMS_INPUT_MOUSE_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_MOUSE_HPP_

#include <array>
#include <cstddef>

#include <SDL2/SDL_mouse.h>

namespace ne
{
namespace mouse
{
inline void                       set_visible          (bool visible )
{
  SDL_ShowCursor(static_cast<int>(visible));
}
inline void                       set_captured         (bool captured)
{
  SDL_CaptureMouse(static_cast<SDL_bool>(captured));
}

inline void                       set_relative         (bool relative)
{
  SDL_SetRelativeMouseMode(static_cast<SDL_bool>(relative));
}
inline bool                       relative             ()
{
  return static_cast<bool>(SDL_GetRelativeMouseMode());
}

inline void                       set_absolute_position(const std::array<std::size_t, 2>& position)
{
  SDL_WarpMouseGlobal(static_cast<int>(position[0]), static_cast<int>(position[1]));
}
inline std::array<std::size_t, 2> absolute_position    ()
{
  std::array<std::size_t, 2> position;
  SDL_GetGlobalMouseState(reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1]));
  return position;
}

inline std::array<std::size_t, 2> relative_position    (bool delta = false)
{
  std::array<std::size_t, 2> position;
  delta ?
    SDL_GetRelativeMouseState(reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1])): 
    SDL_GetMouseState        (reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1]));
  return position;
}

inline bool                       is_button_pressed    (std::size_t button)
{
  return SDL_GetGlobalMouseState(nullptr, nullptr) & static_cast<Uint32>(button);
}
}
}

#endif
