#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_

#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include <nano_engine/systems/display/display_info.hpp>
#include <nano_engine/systems/display/opengl_window.hpp>
#include <nano_engine/systems/display/window.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class display_system final : public system
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

  window*                          create_window        (const std::string& name)
  {
    windows_.emplace_back(std::make_unique<window>(name));
    windows_.back()->owner_ = this;
    return windows_.back().get();
  }
  opengl_window*                   create_opengl_window (const std::string& name, const opengl_context_settings& context_settings = opengl_context_settings())
  {
    opengl_windows_.emplace_back(std::make_unique<opengl_window>(name, context_settings));
    opengl_windows_.back()->owner_ = this;
    return opengl_windows_.back().get();
  }

  void                             destroy_window       (window*        window       )
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
  void                             destroy_opengl_window(opengl_window* opengl_window)
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

  std::vector<window*>             windows              () const
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
  std::vector<opengl_window*>      opengl_windows       () const
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
  
  static std::string               current_video_driver ()
  {
    return std::string(SDL_GetCurrentVideoDriver());
  }
  static std::vector<std::string>  video_drivers        ()
  {
    std::vector<std::string> drivers;
    for(auto i = 0; i < SDL_GetNumVideoDrivers(); ++i)
      drivers.emplace_back(SDL_GetVideoDriver(i));
    return drivers;
  }
  static std::vector<display_info> displays             ()
  {
    std::vector<display_info> displays;
    for(auto i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
      auto name = std::string(SDL_GetDisplayName(i));

      SDL_Rect bounds, usable_bounds;
      SDL_GetDisplayBounds      (i, &bounds       );
      SDL_GetDisplayUsableBounds(i, &usable_bounds);

      std::array<float, 3> dpi;
      SDL_GetDisplayDPI(i, &dpi[2], &dpi[0], &dpi[1]);
      
      std::vector<display_mode> display_modes;
      for(auto j = 0; j < SDL_GetNumDisplayModes(i); ++j)
      {
        SDL_DisplayMode native_display_mode;
        SDL_GetDisplayMode(i, j, &native_display_mode);
        display_modes.emplace_back(native_display_mode);
      }

      SDL_DisplayMode native_current_display_mode, native_desktop_display_mode;
      SDL_GetCurrentDisplayMode(i, &native_current_display_mode);
      SDL_GetDesktopDisplayMode(i, &native_desktop_display_mode);

      displays.push_back({
        static_cast<std::size_t>(i),
        name,
        {static_cast<std::size_t>(bounds       .w), static_cast<std::size_t>(bounds       .h)},
        {static_cast<std::size_t>(usable_bounds.w), static_cast<std::size_t>(usable_bounds.h)},
        dpi,
        display_modes,
        display_mode(native_current_display_mode),
        display_mode(native_desktop_display_mode)
      });
    }
    return displays;
  }
  
private:
  void tick() override
  {
    for(auto& window : opengl_windows_)
      window->swap();
  }

  std::vector<std::unique_ptr<window>>        windows_       ;
  std::vector<std::unique_ptr<opengl_window>> opengl_windows_;
};

inline display_info window::display() const
{
  return owner_->displays()[SDL_GetWindowDisplayIndex(native_)];
}
}

#endif
