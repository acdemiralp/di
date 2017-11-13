#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_WINDOW_HPP_

#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

namespace ne
{
class window
{
public:
  enum class mode
  {
    windowed  ,
    fullscreen,
    fullscreen_windowed
  };

  window           (const std::string& title, const std::array<std::size_t, 2>& position, const std::array<std::size_t, 2>& size, std::uint32_t flags = 0u)
  {
    native_ = SDL_CreateWindow(
      title.c_str(), 
      static_cast<int>(position[0]), 
      static_cast<int>(position[1]), 
      static_cast<int>(size    [0]), 
      static_cast<int>(size    [1]), 
      flags |= SDL_WINDOW_ALLOW_HIGHDPI);
    if (!native_)
      throw std::runtime_error("Failed to create SDL window. SDL Error: " + std::string(SDL_GetError()));
  }
  explicit window  (const std::string& title, std::uint32_t flags = 0u) : window(title, {32, 32}, {800, 600}, flags)
  {
    set_fullscreen_windowed();
  }
  window           (const window&  that) = delete ;
  window           (      window&& temp) = default;
  virtual ~window  ()
  {
    SDL_DestroyWindow(native_ );
  }
  window& operator=(const window&  that) = delete ;
  window& operator=(      window&& temp) = default;

  void bring_to_front() const
  {
    SDL_RaiseWindow(native_);
  }
  void minimize      () const
  {
    SDL_MinimizeWindow(native_);
  }
  void maximize      () const
  {
    SDL_MaximizeWindow(native_);
  }
  void restore       () const
  {
    SDL_RestoreWindow(native_);
  }
  bool is_grabbed    () const
  {
    return SDL_GetGrabbedWindow() == native_;
  }

  void set_title     (const std::string&                name      )
  {
    SDL_SetWindowTitle(native_, name.c_str());
  }
  void set_position  (const std::array<std::size_t, 2>& position  )
  {
    SDL_SetWindowPosition(native_, static_cast<int>(position[0]), static_cast<int>(position[1]));
  }
  void set_size      (const std::array<std::size_t, 2>& size      )
  {
    SDL_SetWindowSize(native_, static_cast<int>(size[0]), static_cast<int>(size[1]));
    on_resize(size);
  }
  void set_resizable (bool                              resizable )
  {
    SDL_SetWindowResizable(native_, SDL_bool(resizable));
  }
  

  ////////

  void set_mode      (mode                           mode      )
  {
    mode_ = mode;
    if (native_)
    {
      if      (mode_ == mode::windowed)
        SDL_SetWindowFullscreen(native_, false);
      else if (mode_ == mode::fullscreen)
        SDL_SetWindowFullscreen(native_, true);
      else if (mode_ == mode::fullscreen_windowed)
      {
        SDL_SetWindowFullscreen(native_, false);
        set_fullscreen_windowed();
      }
      on_resize(get_size());
    }
  }
  void set_hidden    (bool                           hidden    )
  {
    hidden_ = hidden;
    if (native_)
      hidden_ ? SDL_HideWindow(native_) : SDL_ShowWindow(native_);
  }
  void set_borderless(bool                           borderless)
  {
    borderless_ = borderless;
    if (native_)
      SDL_SetWindowBordered(native_, SDL_bool(!borderless));
  }
  void set_minimized (bool                           minimized )
  {
    minimized_ = minimized;
    if (minimized_ && native_)
    {
      SDL_MinimizeWindow(native_);
      on_resize(get_size());
    }
  }
  void set_maximized (bool                           maximized )
  {
    maximized_ = maximized;
    if (maximized_ && native_)
    {
      SDL_MaximizeWindow(native_);
      on_resize(get_size());
    }
  }
  void set_grab_input(bool                           grab_input)
  {
    grab_input_ = grab_input;
    if (native_)
      SDL_SetWindowGrab(native_, SDL_bool(grab_input_));
  }
  
  const std::string&      get_name      () const { return name_      ; }
  std::array<unsigned, 2> get_position  ()
  {
    if (native_)
      SDL_GetWindowPosition(native_, reinterpret_cast<int*>(&position_[0]), reinterpret_cast<int*>(&position_[1]));
    return position_;
  }
  std::array<unsigned, 2> get_size      ()
  {
    if(native_)
      SDL_GetWindowSize(native_, reinterpret_cast<int*>(&size_[0]), reinterpret_cast<int*>(&size_[1]));
    return size_;
  }
  mode                    get_mode      () const { return mode_      ; }
  bool                    get_hidden    () const { return hidden_    ; }
  bool                    get_borderless() const { return borderless_; }
  bool                    get_resizable () const { return resizable_ ; }
  bool                    get_minimized () const { return minimized_ ; }
  bool                    get_maximized () const { return maximized_ ; }
  bool                    get_grab_input() const { return grab_input_; }
  
  SDL_Window*             native        () const { return native_    ; }

  boost::signals2::signal<void(const std::array<std::size_t, 2>&)> on_resize;

protected:
  void set_fullscreen_windowed()
  {
    SDL_DisplayMode video_mode;
    SDL_GetCurrentDisplayMode(0, &video_mode);
    set_position(std::array<std::size_t, 2>{0u, 0u});
    set_size    (std::array<std::size_t, 2>{video_mode.w - 1, video_mode.h - 1});
  }

  SDL_Window* native_ = nullptr;
};
}

#endif
