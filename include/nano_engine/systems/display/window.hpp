#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_WINDOW_HPP_

#include <array>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>

namespace ne
{
class window
{
public:
  struct context_settings
  {
    enum class profile : unsigned
    {
      core             = 0x0001,
      compatibility    = 0x0002,
      embedded_systems = 0x0004
    };

    unsigned major_version = 4;
    unsigned minor_version = 5;
    profile  profile       = profile::core;
  };

  enum class mode
  {
    windowed  ,
    fullscreen,
    fullscreen_windowed
  };
  enum class swap_mode : int
  {
    late_swap_tearing = -1,
    immediate         =  0,
    vertical_sync     =  1
  };

  window           (const context_settings& settings = context_settings())
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, settings.major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, settings.minor_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , static_cast<int>(settings.profile));

    unsigned int flags = SDL_WINDOW_OPENGL;
    
    if      (mode_ == mode::fullscreen)
      flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    else if (mode_ == mode::fullscreen_windowed)
      set_fullscreen_windowed();
    
    if (hidden_    ) flags |= SDL_WINDOW_HIDDEN       ;
    if (borderless_) flags |= SDL_WINDOW_BORDERLESS   ;
    if (resizable_ ) flags |= SDL_WINDOW_RESIZABLE    ;
    if (minimized_ ) flags |= SDL_WINDOW_MINIMIZED    ;
    if (maximized_ ) flags |= SDL_WINDOW_MAXIMIZED    ;
    if (grab_input_) flags |= SDL_WINDOW_INPUT_GRABBED;

    native_ = SDL_CreateWindow(name_.c_str(), position_[0], position_[1], size_[0], size_[1], flags);
    if (!native_)
    {
      std::cout << "Failed to create the window. SDL Error: " << SDL_GetError() << std::endl;
      return;
    }

    context_ = SDL_GL_CreateContext(native_);
    if (!context_)
    {
      std::cout << "Failed to create the context. SDL Error: " << SDL_GetError() << std::endl;
      return;
    }
  }
  window           (const window&  that) = delete ;
  window           (      window&& temp) = default;
  virtual ~window  ()
  {
    if (context_)
      SDL_GL_DeleteContext(context_);
    if (native_)
      SDL_DestroyWindow   (native_ );
  }
  window& operator=(const window&  that) = delete ;
  window& operator=(      window&& temp) = default;

  void swap_buffers() const
  {
    if (native_)
      SDL_GL_SwapWindow(native_);
    else
      std::cout << "Failed to swap buffers. Display does not have a window." << std::endl;
  }

  static void set_swap_mode(swap_mode swap_mode)
  {
    if (SDL_GL_SetSwapInterval(static_cast<int>(swap_mode)) != 0)
      std::cout << "Failed to set the swap interval. SDL Error: " << SDL_GetError() << std::endl;
  }

  void set_name      (const std::string&             name      )
  {
    name_ = name;
    if (native_)
      SDL_SetWindowTitle(native_, name_.c_str());
  }
  void set_position  (const std::array<unsigned, 2>& position  )
  {
    position_ = position;
    if (native_)
      SDL_SetWindowPosition(native_, position_[0], position_[1]);
  }
  void set_size      (const std::array<unsigned, 2>& size      )
  {
    size_ = size;
    if (native_)
    {
      SDL_SetWindowSize(native_, size_[0], size_[1]);
      on_resize(size_);
    }
  }
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
  void set_resizable (bool                           resizable )
  {
    resizable_ = resizable;
    if (native_)
      SDL_SetWindowResizable(native_, SDL_bool(resizable));
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
  
  boost::signals2::signal<void(const std::array<unsigned, 2>&)> on_resize;

protected:
  void set_fullscreen_windowed()
  {
    SDL_DisplayMode video_mode;
    SDL_GetCurrentDisplayMode(0, &video_mode);
    set_position(std::array<unsigned, 2>{0u, 0u});
    set_size    (std::array<unsigned, 2>{unsigned(video_mode.w - 1), unsigned(video_mode.h - 1)});
  }

  std::string                 name_       = "";
  std::array<unsigned, 2>     position_   = std::array<unsigned, 2>{32u , 32u };
  std::array<unsigned, 2>     size_       = std::array<unsigned, 2>{800u, 600u};
  mode                        mode_       = mode::windowed;
  bool                        fullscreen_ = false;
  bool                        hidden_     = false;
  bool                        borderless_ = false;
  bool                        resizable_  = true ;
  bool                        minimized_  = false;
  bool                        maximized_  = false;
  bool                        grab_input_ = false;
  
  SDL_Window*                 native_     = nullptr;
  SDL_GLContext               context_    = nullptr;
};
}

#endif
