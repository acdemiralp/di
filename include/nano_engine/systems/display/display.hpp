#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_HPP_

#include <iostream>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include <nano_engine/systems/display/window.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class display : public system
{
public:
  display           ()
  {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
      std::cout << "Failed to initialize display system. SDL Error: " << SDL_GetError() << std::endl;
  }
  display           (const display&  that) = default;
  display           (      display&& temp) = default;
  virtual ~display  ()
  {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
  display& operator=(const display&  that) = default;
  display& operator=(      display&& temp) = default;

  window* create_window(const window::context_settings& settings = window::context_settings())
  {
    windows_.push_back(std::make_unique<window>(settings));
    return windows_.back().get();
  }
  
  std::vector<window*> get_windows() const
  {
    std::vector<window*> windows(windows_.size());
    std::transform(
      windows_.begin(), 
      windows_.end  (), 
      windows .begin(), 
      [ ] (const std::unique_ptr<window>& window)
      {
        return window.get();
      });
    return windows;
  }

protected:
  void update() override
  {
    for(auto& window : windows_)
      window->swap_buffers();
  }

  std::vector<std::unique_ptr<window>> windows_;
};
}

#endif
