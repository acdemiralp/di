#ifndef NANO_ENGINE_SYSTEMS_INPUT_GAME_CONTROLLER_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_GAME_CONTROLLER_HPP_

#include <cstddef>
#include <stdexcept>

#include <SDL2/SDL_gamecontroller.h>

namespace ne
{
class game_controller
{
public:
  game_controller           (std::size_t index) : native_(SDL_GameControllerOpen(index))
  {
    if(!native_)
      throw std::runtime_error("Failed to create SDL game controller. SDL Error: " + std::string(SDL_GetError()));
  }
  game_controller           (const game_controller&  that) = delete ;
  game_controller           (      game_controller&& temp) = default;
  ~game_controller          ()
  {
    SDL_GameControllerClose(native_);
  }
  game_controller& operator=(const game_controller&  that) = delete ;
  game_controller& operator=(      game_controller&& temp) = default;

private:
  SDL_GameController* native_;
};
}

#endif
