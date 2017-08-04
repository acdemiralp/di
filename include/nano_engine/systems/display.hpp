#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_HPP_

#include <SDL2/SDL.h>

#include <nano_engine/system.hpp>

namespace ne
{
class display : public system
{
public:

protected:
  friend class gl_context;

  void initialize() override
  {
    
  }
  void update    () override
  {
    
  }
  void terminate () override
  {
    
  }

  SDL_Window* window_;
};
}

#endif
