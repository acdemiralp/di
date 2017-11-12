#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_

#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
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
  enum class message_box_level : unsigned
  {
    error       = SDL_MESSAGEBOX_ERROR,
    warning     = SDL_MESSAGEBOX_WARNING,
    information = SDL_MESSAGEBOX_INFORMATION
  };

  struct display_mode
  {
    explicit display_mode(const SDL_DisplayMode& native) 
    : format      (native.format)
    , size        ({static_cast<std::size_t>(native.w), static_cast<std::size_t>(native.h)})
    , refresh_rate( static_cast<std::size_t>(native.refresh_rate))
    {

    }
    display_mode ()                                    = default;
    display_mode (const display_mode&  that)           = default;
    display_mode (      display_mode&& temp)           = default;
    ~display_mode()                                    = default;
    display_mode& operator=(const display_mode&  that) = default;
    display_mode& operator=(      display_mode&& temp) = default;
    
    SDL_DisplayMode to_native() const
    {
      return {format, static_cast<int>(size[0]), static_cast<int>(size[1]), static_cast<int>(refresh_rate), nullptr};
    }

    std::uint32_t              format      ;
    std::array<std::size_t, 2> size        ;
    std::size_t                refresh_rate;
  };
  struct display_info
  {
    display_mode closest_display_mode(const display_mode& mode) const
    {
      auto native_mode = mode.to_native();
      SDL_DisplayMode native_closest_mode;
      SDL_GetClosestDisplayMode(static_cast<int>(index), &native_mode, &native_closest_mode);
      return display_mode(native_closest_mode);
    }

    std::size_t                index               ;
    std::string                name                ;
    std::array<std::size_t, 2> size                ;
    std::array<std::size_t, 2> usable_size         ;
    std::array<float      , 3> dpi                 ; // width, height, diagonal
    std::vector<display_mode>  display_modes       ;
    display_mode               current_display_mode;
    display_mode               desktop_display_mode;
  };
  struct message_box_color_scheme
  {
    std::array<std::uint8_t, 3> background        = { 200, 200, 200 };
    std::array<std::uint8_t, 3> text              = {   0,   0,   0 };
    std::array<std::uint8_t, 3> button_border     = { 175, 175, 175 };
    std::array<std::uint8_t, 3> button_background = { 100, 100, 100 };
    std::array<std::uint8_t, 3> button_selected   = { 150, 150, 150 };

    SDL_MessageBoxColorScheme to_native() const
    {
      return SDL_MessageBoxColorScheme
      {{
        { background       [0], background       [1], background       [2]},
        { text             [0], text             [1], text             [2]},
        { button_border    [0], button_border    [1], button_border    [2]},
        { button_background[0], button_background[1], button_background[2]},
        { button_selected  [0], button_selected  [1], button_selected  [2]}
      }};
    }
  };

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

  // Factory functionality.
  window*                     create_window        ()
  {
    windows_.emplace_back(std::make_unique<window>());
    return windows_.back().get();
  }
  opengl_window*              create_opengl_window (const opengl_window::context_settings& context_settings = opengl_window::context_settings())
  {
    opengl_windows_.emplace_back(std::make_unique<opengl_window>(context_settings));
    return opengl_windows_.back().get();
  }
  void                        destroy_window       (window*        window       )
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
  void                        destroy_opengl_window(opengl_window* opengl_window)
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
  std::vector<window*>        windows              () const
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
  std::vector<opengl_window*> opengl_windows       () const
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
  
  // Auxiliary functionality.
  static void                       create_message_box     (
    const std::string&               title        , 
    const std::string&               message      , 
    const message_box_level&         level        = message_box_level::information)
  {
    SDL_ShowSimpleMessageBox(static_cast<Uint32>(level), title.c_str(), message.c_str(), nullptr);
  }
  static std::string                create_message_box     (
    const std::string&               title        , 
    const std::string&               message      , 
    const std::vector<std::string>&  buttons      , 
    const message_box_level&         level        = message_box_level::information, 
    const message_box_color_scheme&  color_scheme = message_box_color_scheme())
  {
    std::vector<SDL_MessageBoxButtonData> native_buttons;
    for(int i = buttons.size() - 1; i >= 0; --i)
      native_buttons.push_back({0, i, buttons[i].c_str()});

    auto native_color_scheme = color_scheme.to_native();

    const SDL_MessageBoxData message_box_data
    {
      static_cast<Uint32>(level)      , 
      nullptr                         , 
      title  .c_str()                 , 
      message.c_str()                 ,
      static_cast<int>(buttons.size()),
      native_buttons.data()           ,
      &native_color_scheme
    };

    auto index = 0;
    SDL_ShowMessageBox(&message_box_data, &index);
    return buttons[index];
  }
  static std::string                current_video_driver   ()
  {
    return std::string(SDL_GetCurrentVideoDriver());
  }
  static std::vector<std::string>   video_drivers          ()
  {
    std::vector<std::string> drivers;
    for(auto i = 0; i < SDL_GetNumVideoDrivers(); ++i)
      drivers.emplace_back(SDL_GetVideoDriver(i));
    return drivers;
  }
  static std::vector<display_info>  displays               ()
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
  static void                       set_screensaver_enabled(bool enabled)
  {
    enabled ? SDL_EnableScreenSaver() : SDL_DisableScreenSaver();
  }
  static bool                       screensaver_enabled    ()
  {
    return SDL_IsScreenSaverEnabled() != 0;
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
