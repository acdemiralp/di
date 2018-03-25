#ifndef DI_SYSTEMS_DISPLAY_WINDOW_HPP_
#define DI_SYSTEMS_DISPLAY_WINDOW_HPP_

#include <array>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <di/systems/display/display_info.hpp>
#include <di/systems/display/hit_test_result.hpp>
#include <di/systems/display/window_flags.hpp>
#include <di/systems/display/window_mode.hpp>
#include <di/utility/rectangle.hpp>

namespace di
{
extern "C" inline SDL_HitTestResult hit_test_callback(SDL_Window* native, const SDL_Point* point, void* data);

class window
{
public:
  explicit window  (const std::string& title, const window_flags flags = window_flags::none)
  : window(title, {32, 32}, {800, 600}, flags)
  {
    set_fullscreen_windowed();
  }
  window           (const std::string& title, const std::array<std::size_t, 2>& position, const std::array<std::size_t, 2>& size, window_flags flags = window_flags::none) 
  : native_(SDL_CreateWindow(title.c_str(), static_cast<int>(position[0]), static_cast<int>(position[1]), static_cast<int>(size[0]), static_cast<int>(size[1]), static_cast<Uint32>(flags)))
  {
    if (!native_)
      throw std::runtime_error("Failed to create SDL window. SDL Error: " + std::string(SDL_GetError()));
  }
  window           (const window&  that) = delete ;
  window           (      window&& temp) noexcept 
  : on_visibility_change    (std::move(temp.on_visibility_change    ))
  , on_expose               (std::move(temp.on_expose               ))
  , on_move                 (std::move(temp.on_move                 ))
  , on_resize               (std::move(temp.on_resize               ))
  , on_minimize             (std::move(temp.on_minimize             ))
  , on_maximize             (std::move(temp.on_maximize             ))
  , on_restore              (std::move(temp.on_restore              ))
  , on_mouse_focus_change   (std::move(temp.on_mouse_focus_change   ))
  , on_keyboard_focus_change(std::move(temp.on_keyboard_focus_change))
  , on_drop_file            (std::move(temp.on_drop_file            ))
  , on_drop_text            (std::move(temp.on_drop_text            ))
  , on_drop_start           (std::move(temp.on_drop_start           ))
  , on_drop_end             (std::move(temp.on_drop_end             ))
  , on_close                (std::move(temp.on_close                ))
  , native_                 (std::move(temp.native_                 ))
  , hit_test_callback_      (std::move(temp.hit_test_callback_      ))
  {
    if(hit_test_callback_ != nullptr)
    {
      SDL_SetWindowHitTest(temp. native_, nullptr, nullptr);
      SDL_SetWindowHitTest(this->native_, static_cast<SDL_HitTest>(hit_test_callback), reinterpret_cast<void*>(this));
      temp.hit_test_callback_ = nullptr;
    }

    temp.native_ = nullptr;
  }
  virtual ~window  ()
  {
    if (hit_test_callback_)
      SDL_SetWindowHitTest(native_, nullptr, nullptr);
    if (native_)
      SDL_DestroyWindow(native_);
  }
  window& operator=(const window&  that) = delete ;
  window& operator=(      window&& temp) noexcept
  {
    if (this != &temp)
    {
      on_visibility_change    = std::move(temp.on_visibility_change    );
      on_expose               = std::move(temp.on_expose               );
      on_move                 = std::move(temp.on_move                 );
      on_resize               = std::move(temp.on_resize               );
      on_minimize             = std::move(temp.on_minimize             );
      on_maximize             = std::move(temp.on_maximize             );
      on_restore              = std::move(temp.on_restore              );
      on_mouse_focus_change   = std::move(temp.on_mouse_focus_change   );
      on_keyboard_focus_change= std::move(temp.on_keyboard_focus_change);
      on_drop_file            = std::move(temp.on_drop_file            );
      on_drop_text            = std::move(temp.on_drop_text            );
      on_drop_start           = std::move(temp.on_drop_start           );
      on_drop_end             = std::move(temp.on_drop_end             );
      on_close                = std::move(temp.on_close                );
      native_                 = std::move(temp.native_                 );
      hit_test_callback_      = std::move(temp.hit_test_callback_      );

      if (hit_test_callback_ != nullptr)
      {
        SDL_SetWindowHitTest(temp.native_, nullptr, nullptr);
        SDL_SetWindowHitTest(this->native_, static_cast<SDL_HitTest>(hit_test_callback), reinterpret_cast<void*>(this));
        temp.hit_test_callback_ = nullptr;
      }

      temp.native_ = nullptr;
    }
    return *this;
  }

  virtual void update() { }

  void set_focus         () const
  {
    SDL_SetWindowInputFocus(native_);
  }
  void bring_to_front    () const
  {
    SDL_RaiseWindow(native_);
  }
  void minimize          () const
  {
    SDL_MinimizeWindow(native_);
  }
  void maximize          () const
  {
    SDL_MaximizeWindow(native_);
  }
  void restore           () const
  {
    SDL_RestoreWindow(native_);
  }
  void set_mouse_position(const std::array<std::size_t, 2>& position) const
  {
    SDL_WarpMouseInWindow(native_, static_cast<int>(position[0]), static_cast<int>(position[1]));
  }

  void set_visible     (bool                                                 visible           )
  {
    visible ? SDL_ShowWindow(native_) : SDL_HideWindow(native_);
  }
  void set_resizable   (bool                                                 resizable         )
  {
    SDL_SetWindowResizable(native_, SDL_bool(resizable));
  }
  void set_bordered    (bool                                                 bordered          )
  {
    SDL_SetWindowBordered(native_, SDL_bool(bordered));
  }
  void set_input_grab  (bool                                                 input_grab        )
  {
    SDL_SetWindowGrab(native_, SDL_bool(input_grab));
  }
  void set_opacity     (float                                                opacity           )
  {
    SDL_SetWindowOpacity(native_, opacity);
  }
  void set_brightness  (float                                                brightness        )
  {
    SDL_SetWindowBrightness(native_, brightness);
  }
  void set_title       (const std::string&                                   title             )
  {
    SDL_SetWindowTitle(native_, title.c_str());
  }
  void set_position    (const std::array<std::size_t, 2>&                    position          )
  {
    SDL_SetWindowPosition(native_, static_cast<int>(position[0]), static_cast<int>(position[1]));
  }
  void set_size        (const std::array<std::size_t, 2>&                    size              )
  {
    SDL_SetWindowSize(native_, static_cast<int>(size[0]), static_cast<int>(size[1]));
  }
  void set_minimum_size(const std::array<std::size_t, 2>&                    minimum_size      )
  {
    SDL_SetWindowMinimumSize(native_, static_cast<int>(minimum_size[0]), static_cast<int>(minimum_size[1]));
  }
  void set_maximum_size(const std::array<std::size_t, 2>&                    maximum_size      )
  {
    SDL_SetWindowMaximumSize(native_, static_cast<int>(maximum_size[0]), static_cast<int>(maximum_size[1]));
  }
  void set_gamma_ramp  (const std::array<std::array<std::uint16_t, 256>, 3>& translation_tables)
  {
    SDL_SetWindowGammaRamp(
      native_,
      translation_tables[0].data(), 
      translation_tables[1].data(), 
      translation_tables[2].data());
  } 
  void set_display_mode(const display_mode&                                  display_mode      )
  {
    auto native_display_mode = display_mode.native();
    SDL_SetWindowDisplayMode(native_, &native_display_mode);
  }
  void set_mode        (window_mode                                          mode              )
  {
    SDL_SetWindowFullscreen(native_, mode == window_mode::fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    if (mode == window_mode::fullscreen_windowed)
      set_fullscreen_windowed();
  }
  void set_parent      (window*                                              parent            )
  {
    SDL_SetWindowModalFor(native_, parent->native_);
  }
  void set_icon        (const std::array<std::size_t, 2>& dimensions, const std::vector<std::uint8_t>& pixels)
  {
    const auto surface = SDL_CreateRGBSurfaceWithFormatFrom(
      const_cast<void*>(reinterpret_cast<const void*>(pixels.data())), 
      static_cast<int>(dimensions[0]), 
      static_cast<int>(dimensions[1]), 
      1, 
      static_cast<int>(dimensions[0] * 4), 
      SDL_PIXELFORMAT_RGBA32);
    SDL_SetWindowIcon(native_, surface);
    SDL_FreeSurface(surface);
  }
  void set_hit_test    (const std::function<hit_test_result(std::array<std::size_t, 2>)>& callback)
  {
    hit_test_callback_ = callback;
    SDL_SetWindowHitTest(native_, hit_test_callback_ != nullptr ? static_cast<SDL_HitTest>(hit_test_callback) : nullptr, reinterpret_cast<void*>(this));
  }

  bool                                          visible         () const
  {
    return (SDL_GetWindowFlags(native_) & SDL_WINDOW_SHOWN) != 0;
  }
  bool                                          resizable       () const
  {
    return (SDL_GetWindowFlags(native_) & SDL_WINDOW_RESIZABLE) != 0;
  }
  bool                                          bordered        () const
  {
    return !(SDL_GetWindowFlags(native_) & SDL_WINDOW_BORDERLESS);
  }
  bool                                          input_grab      () const
  {
    return SDL_GetWindowGrab(native_) != 0;
  }
  bool                                          input_focus     () const
  {
    return (SDL_GetWindowFlags(native_) & SDL_WINDOW_INPUT_FOCUS) != 0;
  }
  bool                                          mouse_focus     () const
  {
    return (SDL_GetWindowFlags(native_) & SDL_WINDOW_MOUSE_FOCUS) != 0;
  }
  bool                                          keyboard_visible() const
  {
    return SDL_IsScreenKeyboardShown(native_) != 0;
  }
  float                                         opacity         () const
  {
    float opacity;
    SDL_GetWindowOpacity(native_, &opacity);
    return opacity;
  }
  float                                         brightness      () const
  {
    return SDL_GetWindowBrightness(native_);
  }
  std::string                                   title           () const
  {
    return std::string(SDL_GetWindowTitle(native_));
  }
  std::array<std::uint32_t, 2>                  position        () const
  {
    std::array<std::uint32_t, 2> position;
    SDL_GetWindowPosition(native_, reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1]));
    return position;
  }
  std::array<std::uint32_t, 2>                  size            () const
  {
    std::array<std::uint32_t, 2> size;
    SDL_GetWindowSize(native_, reinterpret_cast<int*>(&size[0]), reinterpret_cast<int*>(&size[1]));
    return size;
  }
  std::array<std::uint32_t, 2>                  minimum_size    () const
  {
    std::array<std::uint32_t, 2> minimum_size;
    SDL_GetWindowMinimumSize(native_, reinterpret_cast<int*>(&minimum_size[0]), reinterpret_cast<int*>(&minimum_size[1]));
    return minimum_size;
  }
  std::array<std::uint32_t, 2>                  maximum_size    () const
  {
    std::array<std::uint32_t, 2> maximum_size;
    SDL_GetWindowMaximumSize(native_, reinterpret_cast<int*>(&maximum_size[0]), reinterpret_cast<int*>(&maximum_size[1]));
    return maximum_size;
  }
  rectangle<std::uint32_t>                      border_size     () const
  {
    rectangle<std::uint32_t> border_size;
    SDL_GetWindowBordersSize(native_, reinterpret_cast<int*>(&border_size.top), reinterpret_cast<int*>(&border_size.left), reinterpret_cast<int*>(&border_size.bottom), reinterpret_cast<int*>(&border_size.right));
    return border_size;
  }
  std::array<std::array<std::uint16_t, 256>, 3> gamma_ramp      () const
  {
    std::array<std::array<std::uint16_t, 256>, 3> translation_tables;
    SDL_GetWindowGammaRamp(
      native_, 
      translation_tables[0].data(),
      translation_tables[1].data(),
      translation_tables[2].data());
    return translation_tables;
  }
  display_mode                                  display_mode    () const
  {
    SDL_DisplayMode native_display_mode;
    SDL_GetWindowDisplayMode(native_, &native_display_mode);
    return di::display_mode(native_display_mode);
  }
  display_info                                  display         () const
  {
    return displays()[SDL_GetWindowDisplayIndex(native_)];
  }
  window_mode                                   mode            () const
  {
    if (SDL_GetWindowFlags(native_) & SDL_WINDOW_FULLSCREEN_DESKTOP)
      return window_mode::fullscreen;
    
    auto display_info = display();
    if(position() == std::array<std::uint32_t, 2>{0, 0} && 
       size    () == std::array<std::uint32_t, 2>{std::uint32_t(display_info.size[0] - 1), std::uint32_t(display_info.size[1] - 1)})
      return window_mode::fullscreen_windowed;

    return window_mode::windowed;
  }

  SDL_Window*   native   () const 
  {
    return native_;
  }
  std::uint32_t native_id() const
  {
    return SDL_GetWindowID(native_);
  }

#if   defined(SDL_VIDEO_DRIVER_ANDROID)
  std::tuple<ANativeWindow*, EGLSurface> driver_data() const
  {
    auto data = driver_specific_data();
    return {data.info.android.window, data.info.android.surface};
  }
#elif defined(SDL_VIDEO_DRIVER_MIR)
  std::tuple<MirConnection*, MirSurface*> driver_data() const
  {
    auto data = driver_specific_data();
    return {data.info.mir.connection, data.info.mir.surface};
  }
#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
  std::tuple<wl_display*, wl_surface*, wl_shell_surface*> driver_data() const
  {
    auto data = driver_specific_data();
    return {data.info.wl.display, data.info.wl.surface, data.info.wl.shell_surface};
  }
#elif defined(SDL_VIDEO_DRIVER_WINDOWS)
  std::tuple<HWND, HDC, HINSTANCE> driver_data() const
  {
    auto data = driver_specific_data();
    return {data.info.win.window, data.info.win.hdc, data.info.win.hinstance};
  }
#elif defined(SDL_VIDEO_DRIVER_X11)
  std::tuple<Display*, Window> driver_data() const
  {
    auto data = driver_specific_data();
    return {data.info.x11.display, data.info.x11.window};
  }
#endif

  boost::signals2::signal<void(bool)>                              on_visibility_change    ;
  boost::signals2::signal<void()>                                  on_expose               ;
  boost::signals2::signal<void(const std::array<std::size_t, 2>&)> on_move                 ;
  boost::signals2::signal<void(const std::array<std::size_t, 2>&)> on_resize               ;
  boost::signals2::signal<void()>                                  on_minimize             ;
  boost::signals2::signal<void()>                                  on_maximize             ;
  boost::signals2::signal<void()>                                  on_restore              ;
  boost::signals2::signal<void(bool)>                              on_mouse_focus_change   ;
  boost::signals2::signal<void(bool)>                              on_keyboard_focus_change;
  boost::signals2::signal<void(std::string)>                       on_drop_file            ;
  boost::signals2::signal<void(std::string)>                       on_drop_text            ;
  boost::signals2::signal<void(std::string)>                       on_drop_start           ;
  boost::signals2::signal<void(std::string)>                       on_drop_end             ;
  boost::signals2::signal<void()>                                  on_close                ;

protected:
  friend SDL_HitTestResult hit_test_callback(SDL_Window*, const SDL_Point*, void*);

  SDL_SysWMinfo driver_specific_data   () const
  {
    SDL_SysWMinfo sys_wm_info;
    SDL_VERSION(&sys_wm_info.version);
    SDL_GetWindowWMInfo(native_, &sys_wm_info);
    return sys_wm_info;
  }
  void          set_fullscreen_windowed()
  {
    auto display_info = display();
    set_position(std::array<std::size_t, 2>{0, 0});
    set_size    (std::array<std::size_t, 2>{display_info.size[0] - 1, display_info.size[1] - 1});
  }
  
  SDL_Window* native_ = nullptr;
  std::function<hit_test_result(std::array<std::size_t, 2>)> hit_test_callback_ = nullptr;
};

extern "C" inline SDL_HitTestResult hit_test_callback(SDL_Window* native, const SDL_Point* point, void* data)
{
  const auto window = reinterpret_cast<di::window*>(data);
  if  (window == nullptr || window->hit_test_callback_ == nullptr) return SDL_HITTEST_NORMAL;
  return static_cast<SDL_HitTestResult>(window->hit_test_callback_({static_cast<std::size_t>(point->x), static_cast<std::size_t>(point->y)}));
}
}

#endif
