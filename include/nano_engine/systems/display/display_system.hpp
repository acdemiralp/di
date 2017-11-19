#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_

#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

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

  template<typename... argument_types>
  window*              create_window       (argument_types&&... arguments)
  {
    windows_.emplace_back(std::make_unique<window>(arguments...));
    return windows_.back().get();
  }
  template<typename... argument_types>
  opengl_window*       create_opengl_window(argument_types&&... arguments)
  {
    windows_.emplace_back(std::make_unique<opengl_window>(arguments...));
    return static_cast<opengl_window*>(windows_.back().get());
  }
  void                 destroy_window      (window* window)
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
  std::vector<window*> windows             () const
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
  
  boost::signals2::signal<void()> on_render_targets_reset;
  boost::signals2::signal<void()> on_render_device_reset ;

private:
  void tick() override
  {
    std::array<SDL_Event, 128> events;
    std::size_t                count ;    
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), events.size(), SDL_GETEVENT, SDL_WINDOWEVENT         , SDL_WINDOWEVENT        ), count > 0)
    {
      for (std::size_t i = 0; i < count; ++i)
      {
        auto& event  = events[i];        
        auto& window = *std::find_if(windows_.begin(), windows_.end(), [&event] (const std::unique_ptr<ne::window>& iteratee)
        {
          return iteratee->native_id() == event.window.windowID;
        });
        if(window.get() == nullptr) // An event from an SDL window which is not handled by the display system.
          continue; 

        if      (event.window.event == SDL_WINDOWEVENT_SHOWN       ) window->on_visibility_change    (true );
        else if (event.window.event == SDL_WINDOWEVENT_HIDDEN      ) window->on_visibility_change    (false);
        else if (event.window.event == SDL_WINDOWEVENT_EXPOSED     ) window->on_expose               ();
        else if (event.window.event == SDL_WINDOWEVENT_MOVED       ) window->on_move                 ({std::size_t(event.window.data1), std::size_t(event.window.data2)});
        else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) window->on_resize               ({std::size_t(event.window.data1), std::size_t(event.window.data2)});
        else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED   ) window->on_minimize             ();
        else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED   ) window->on_maximize             ();
        else if (event.window.event == SDL_WINDOWEVENT_RESTORED    ) window->on_restore              ();
        else if (event.window.event == SDL_WINDOWEVENT_ENTER       ) window->on_mouse_focus_change   (true );
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE       ) window->on_mouse_focus_change   (false);
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) window->on_keyboard_focus_change(true );
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST  ) window->on_keyboard_focus_change(false);
        else if (event.window.event == SDL_WINDOWEVENT_CLOSE       ) window->on_close                ();
        else if (event.window.event == SDL_WINDOWEVENT_TAKE_FOCUS  ) window->set_focus               ();
      }
    }
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), events.size(), SDL_GETEVENT, SDL_RENDER_TARGETS_RESET, SDL_RENDER_DEVICE_RESET), count > 0)
    {
      for (std::size_t i = 0; i < count; ++i)
      {
        auto& event = events[i];
        if      (event.type == SDL_RENDER_TARGETS_RESET) on_render_targets_reset();
        else if (event.type == SDL_RENDER_DEVICE_RESET ) on_render_device_reset ();
      }
    }
    for(auto& window : windows_)
      window->update();
  }

  std::vector<std::unique_ptr<window>> windows_;
};
}

#endif
