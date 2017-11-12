#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_HPP_

#include <memory>
#include <stdexcept>
#include <vector>

#include <SDL2/SDL.h>

#include <nano_engine/systems/display/opengl_window.hpp>
#include <nano_engine/systems/display/window.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class display_system : public system
{
public:
  display_system           ()
  {
    if (SDL_VideoInit(nullptr) != 0)
      throw std::runtime_error("Failed to initialize SDL Video subsystem. Error: " + std::string(SDL_GetError()));
  }
  display_system           (const display_system&  that) = default;
  display_system           (      display_system&& temp) = default;
  virtual ~display_system  ()
  {
    SDL_VideoQuit();
  }
  display_system& operator=(const display_system&  that) = default;
  display_system& operator=(      display_system&& temp) = default;

  window*        create_window       ()
  {
    windows_.push_back(std::make_unique<window>());
    return windows_.back().get();
  }
  opengl_window* create_opengl_window(const opengl_window::context_settings& context_settings = opengl_window::context_settings())
  {
    opengl_windows_.push_back(std::make_unique<opengl_window>(context_settings));
    return opengl_windows_.back().get();
  }
  
  void destroy_window       (window*        window       )
  {
    windows_.erase(std::remove_if(
      windows_.begin(), 
      windows_.end  (), 
      [&window] (const std::unique_ptr<ne::window>& iteratee)
      {
        return iteratee.get() == window;
      }), 
      windows_.end  ());
  }
  void destroy_opengl_window(opengl_window* opengl_window)
  {
    opengl_windows_.erase(std::remove_if(
      opengl_windows_.begin(),
      opengl_windows_.end  (),
      [&opengl_window] (const std::unique_ptr<ne::opengl_window>& iteratee)
      {
        return iteratee.get() == opengl_window;
      }), 
      opengl_windows_.end  ());
  }

  std::vector<window*>        get_windows       () const
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
  std::vector<opengl_window*> get_opengl_windows() const
  {
    std::vector<opengl_window*> windows(opengl_windows_.size());
    std::transform(
      opengl_windows_.begin(),
      opengl_windows_.end  (),
      windows        .begin(), 
      [ ] (const std::unique_ptr<opengl_window>& window)
      {
        return window.get();
      });
    return windows;
  }

protected:
  void tick() override
  {
    for(auto& window : opengl_windows_)
      window->swap();
  }

  std::vector<std::unique_ptr<window>>        windows_       ;
  std::vector<std::unique_ptr<opengl_window>> opengl_windows_;
};
}

#endif
