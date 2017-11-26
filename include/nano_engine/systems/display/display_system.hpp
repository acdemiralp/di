#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/signals2.hpp>
#include <openvr.h>
#include <SDL2/SDL_video.h>

#include <nano_engine/systems/display/opengl_window.hpp>
#include <nano_engine/systems/display/vulkan_window.hpp>
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

    auto error    = vr::VRInitError_None;
    vr_interface_ = vr::VR_Init(&error, vr::VRApplication_Scene);
    if (error != vr::VRInitError_None)
    {
      // Unable to find HMD. Proceed without.
    }
  }
  display_system           (const display_system&  that) = default;
  display_system           (      display_system&& temp) = default;
  virtual ~display_system  ()
  {
    SDL_VideoQuit();
    if(vr_interface_) vr::VR_Shutdown();
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
  template<typename... argument_types>
  vulkan_window*       create_vulkan_window(argument_types&&... arguments)
  {
    windows_.emplace_back(std::make_unique<vulkan_window>(arguments...));
    return static_cast<vulkan_window*>(windows_.back().get());
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
  
  window* window_with_input_grab    () const
  {
    auto native  = SDL_GetGrabbedWindow();
    auto windows = this->windows();
    for (auto& window : windows)
      if (window->native() == native)
        return window;
    return nullptr;
  }
  window* window_with_keyboard_focus() const
  {
    auto native  = SDL_GetMouseFocus();
    auto windows = this->windows();
    for (auto& window : windows)
      if (window->native() == native)
        return window;
    return nullptr;
  }
  window* window_with_mouse_focus   () const
  {
    auto native  = SDL_GetKeyboardFocus();
    auto windows = this->windows();
    for (auto& window : windows)
      if (window->native() == native)
        return window;
    return nullptr;
  }

  boost::signals2::signal<void()> on_render_targets_reset;
  boost::signals2::signal<void()> on_render_device_reset ;

private:
  void tick() override
  {
    std::array<SDL_Event, 128> events;
    int                        count ;    
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), static_cast<int>(events.size()), SDL_GETEVENT, SDL_WINDOWEVENT         , SDL_WINDOWEVENT        ), count > 0)
    {
      for (auto i = 0; i < count; ++i)
      {
        auto& event  = events[i];        
        auto  window = std::find_if(windows_.begin(), windows_.end(), [&event] (const std::unique_ptr<ne::window>& iteratee)
        {
          return iteratee->native_id() == event.window.windowID;
        });
        if (window == windows_.end()) // An event from an SDL window which is not handled by the display system.
          continue;

        if      (event.window.event == SDL_WINDOWEVENT_SHOWN       ) window->get()->on_visibility_change    (true );
        else if (event.window.event == SDL_WINDOWEVENT_HIDDEN      ) window->get()->on_visibility_change    (false);
        else if (event.window.event == SDL_WINDOWEVENT_EXPOSED     ) window->get()->on_expose               ();
        else if (event.window.event == SDL_WINDOWEVENT_MOVED       ) window->get()->on_move                 ({std::size_t(event.window.data1), std::size_t(event.window.data2)});
        else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) window->get()->on_resize               ({std::size_t(event.window.data1), std::size_t(event.window.data2)});
        else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED   ) window->get()->on_minimize             ();
        else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED   ) window->get()->on_maximize             ();
        else if (event.window.event == SDL_WINDOWEVENT_RESTORED    ) window->get()->on_restore              ();
        else if (event.window.event == SDL_WINDOWEVENT_ENTER       ) window->get()->on_mouse_focus_change   (true );
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE       ) window->get()->on_mouse_focus_change   (false);
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) window->get()->on_keyboard_focus_change(true );
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST  ) window->get()->on_keyboard_focus_change(false);
        else if (event.window.event == SDL_WINDOWEVENT_CLOSE       ) window->get()->on_close                ();
        else if (event.window.event == SDL_WINDOWEVENT_TAKE_FOCUS  ) window->get()->set_focus               ();
      }
    }
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), static_cast<int>(events.size()), SDL_GETEVENT, SDL_DROPFILE            , SDL_DROPCOMPLETE       ), count > 0)
    {
      for (auto i = 0; i < count; ++i)
      {
        auto& event  = events[i];        
        auto  window = std::find_if(windows_.begin(), windows_.end(), [&event] (const std::unique_ptr<ne::window>& iteratee)
        {
          return iteratee->native_id() == event.drop.windowID;
        });
        if (window == windows_.end()) // An event from an SDL window which is not handled by the display system.
          continue;

        if      (event.type == SDL_DROPFILE    ) window->get()->on_drop_file (std::string(event.drop.file));
        else if (event.type == SDL_DROPTEXT    ) window->get()->on_drop_text (std::string(event.drop.file));
        else if (event.type == SDL_DROPBEGIN   ) window->get()->on_drop_start(std::string(event.drop.file));
        else if (event.type == SDL_DROPCOMPLETE) window->get()->on_drop_end  (std::string(event.drop.file));

        SDL_free(event.drop.file);
      }
    }
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), static_cast<int>(events.size()), SDL_GETEVENT, SDL_RENDER_TARGETS_RESET, SDL_RENDER_DEVICE_RESET), count > 0)
    {
      for (auto i = 0; i < count; ++i)
      {
        auto& event = events[i];
        if      (event.type == SDL_RENDER_TARGETS_RESET) on_render_targets_reset();
        else if (event.type == SDL_RENDER_DEVICE_RESET ) on_render_device_reset ();
      }
    }
    for(auto& window : windows_)
      window->update();
  }

  std::vector<std::unique_ptr<window>> windows_     ;
  vr::IVRSystem*                       vr_interface_;
};
}

#endif
