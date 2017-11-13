#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_WINDOW_HPP_

#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

#include <nano_engine/systems/display/window_mode.hpp>

namespace ne
{
class window
{
public:
  window           (const std::string& title, const std::array<std::size_t, 2>& position, const std::array<std::size_t, 2>& size, std::uint32_t flags = 0u) 
  : native_(SDL_CreateWindow(title.c_str(), static_cast<int>(position[0]), static_cast<int>(position[1]), static_cast<int>(size[0]), static_cast<int>(size[1]), flags |= SDL_WINDOW_ALLOW_HIGHDPI))
  {
    if (!native_)
      throw std::runtime_error("Failed to create SDL window. SDL Error: " + std::string(SDL_GetError()));
  }
  explicit window  (const std::string& title, std::uint32_t flags = 0u) 
  : window(title, {32, 32}, {800, 600}, flags)
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

  void set_visible     (bool visible) const
  {
    visible ? SDL_ShowWindow(native_) : SDL_HideWindow(native_);
  }
  void set_focus       ()             const
  {
    SDL_SetWindowInputFocus(native_);
  }
  void bring_to_front  () const
  {
    SDL_RaiseWindow(native_);
  }
  void minimize        () const
  {
    SDL_MinimizeWindow(native_);
  }
  void maximize        () const
  {
    SDL_MaximizeWindow(native_);
  }
  void restore         () const
  {
    SDL_RestoreWindow(native_);
  }
  bool is_grabbed      () const
  {
    return SDL_GetGrabbedWindow() == native_;
  }

  void set_title       (const std::string&                name      )
  {
    SDL_SetWindowTitle(native_, name.c_str());
  }
  void set_position    (const std::array<std::size_t, 2>& position  )
  {
    SDL_SetWindowPosition(native_, static_cast<int>(position[0]), static_cast<int>(position[1]));
  }
  void set_size        (const std::array<std::size_t, 2>& size      )
  {
    SDL_SetWindowSize(native_, static_cast<int>(size[0]), static_cast<int>(size[1]));
    on_resize(this->size());
  }
  void set_minimum_size(const std::array<std::size_t, 2>& size      )
  {
    SDL_SetWindowMinimumSize(native_, static_cast<int>(size[0]), static_cast<int>(size[1]));
    on_resize(this->size());
  }
  void set_maximum_size(const std::array<std::size_t, 2>& size      )
  {
    SDL_SetWindowMaximumSize(native_, static_cast<int>(size[0]), static_cast<int>(size[1]));
    on_resize(this->size());
  }
  void set_resizable   (bool                              resizable )
  {
    SDL_SetWindowResizable(native_, SDL_bool(resizable));
  }
  void set_bordered    (bool                              bordered  )
  {
    SDL_SetWindowBordered(native_, SDL_bool(bordered));
  }
  void set_input_grab  (bool                              input_grab)
  {
    SDL_SetWindowGrab(native_, SDL_bool(input_grab));
  }

  void set_brightness  (float brightness)
  {
    SDL_SetWindowBrightness(native_, brightness);
  }
  void set_opacity     (float opacity   )
  {
    SDL_SetWindowOpacity(native_, opacity);
  }
  void set_gamma_ramp  (const std::array<std::array<std::uint16_t, 256>, 3>& rgb_translation_table)
  {
    SDL_SetWindowGammaRamp(
      native_, 
      rgb_translation_table[0].data(), 
      rgb_translation_table[1].data(), 
      rgb_translation_table[2].data());
  }

  void set_parent      (window*     parent)
  {
    SDL_SetWindowModalFor(native_, parent->native_);
  }
  void set_mode        (window_mode mode  )
  {
    SDL_SetWindowFullscreen(native_, mode == window_mode::fullscreen);
    if (mode == window_mode::fullscreen_windowed)
      set_fullscreen_windowed();
    on_resize(size());
  }

  // TOFIX
  template<typename pixel_type>
  void set_icon(const std::array<std::size_t, 3>& dimensions, const std::vector<pixel_type>& pixels)
  {
    auto surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels.data(), dimensions.x, dimensions.y, dimensions.z, dimensions.x * 4, SDL_PIXELFORMAT_RGBA32);
    SDL_SetWindowIcon(native_, surface);
    SDL_FreeSurface(surface);
  }

  std::string                        title       () const
  {
    return std::string(SDL_GetWindowTitle(native_));
  }
  std::array<std::size_t, 2>         position    () const
  {
    std::array<std::size_t, 2> position;
    SDL_GetWindowPosition(native_, reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1]));
    return position;
  }
  virtual std::array<std::size_t, 2> size        () const
  {
    std::array<std::size_t, 2> size;
    SDL_GetWindowSize(native_, reinterpret_cast<int*>(&size[0]), reinterpret_cast<int*>(&size[1]));
    return size;
  }
  std::array<std::size_t, 2>         minimum_size() const
  {
    std::array<std::size_t, 2> minimum_size;
    SDL_GetWindowMinimumSize(native_, reinterpret_cast<int*>(&minimum_size[0]), reinterpret_cast<int*>(&minimum_size[1]));
    return minimum_size;
  }
  std::array<std::size_t, 2>         maximum_size() const
  {
    std::array<std::size_t, 2> maximum_size;
    SDL_GetWindowMaximumSize(native_, reinterpret_cast<int*>(&maximum_size[0]), reinterpret_cast<int*>(&maximum_size[1]));
    return maximum_size;
  }
  bool                               input_grab  () const
  {
    return static_cast<bool>(SDL_GetWindowGrab(native_));
  }
  std::array<std::size_t, 4>         border_size () const // top, left, bottom, right
  {
    std::array<std::size_t, 4> border_size;
    SDL_GetWindowBordersSize(native_, reinterpret_cast<int*>(&border_size[0]), reinterpret_cast<int*>(&border_size[1]), reinterpret_cast<int*>(&border_size[2]), reinterpret_cast<int*>(&border_size[3]));
    return border_size;
  }

  float brightness() const
  {
    return SDL_GetWindowBrightness(native_);
  }
  float opacity   () const
  {
    float  opacity;
    SDL_GetWindowOpacity(native_, &opacity);
    return opacity;
  }
  std::array<std::array<std::uint16_t, 256>, 3> gamma_ramp() const
  {
    std::array<std::array<std::uint16_t, 256>, 3> rgb_translation_table;
    SDL_GetWindowGammaRamp(
      native_, 
      rgb_translation_table[0].data(),
      rgb_translation_table[1].data(),
      rgb_translation_table[2].data());
    return rgb_translation_table;
  }

  SDL_Window* native() const { return native_; }

  boost::signals2::signal<void(const std::array<std::size_t, 2>&)> on_resize;

protected:
  void set_fullscreen_windowed(std::size_t display_index = 0)
  {
    SDL_DisplayMode video_mode;
    SDL_GetCurrentDisplayMode(static_cast<int>(display_index), &video_mode);
    set_position(std::array<std::size_t, 2>{0u, 0u});
    set_size    (std::array<std::size_t, 2>{std::size_t(video_mode.w - 1), std::size_t(video_mode.h - 1)});
  }

  SDL_Window* native_ = nullptr;
};
}

#endif
