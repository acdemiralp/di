#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_VULKAN_WINDOW_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_VULKAN_WINDOW_HPP_

#include <SDL2/SDL_vulkan.h>

namespace ne
{
// Note: Vulkan must be included prior to including this file.
class vulkan_window final : public window
{
public:
  explicit vulkan_window  (const VkInstance instance, const std::string& title, const window_flags flags = window_flags::none) 
  : window(title, flags), instance_(instance)
  {
    if(!SDL_Vulkan_CreateSurface(native_, instance_, &surface_))
      throw std::runtime_error("Failed to create Vulkan surface. SDL Error: " + std::string(SDL_GetError()));
  }
  vulkan_window           (const VkInstance instance, const std::string& title, const std::array<std::size_t, 2>& position, const std::array<std::size_t, 2>& size, const window_flags flags = window_flags::none)
  : window(title, position, size, flags), instance_(instance)
  {
    if (!SDL_Vulkan_CreateSurface(native_, instance_, &surface_))
      throw std::runtime_error("Failed to create Vulkan surface. SDL Error: " + std::string(SDL_GetError()));
  }
  vulkan_window           (const vulkan_window&  that) = delete ;
  vulkan_window           (      vulkan_window&& temp) = default;
  ~vulkan_window          ()
  {
#ifdef VULKAN_H_
    vkDestroySurfaceKHR(instance_, surface_, nullptr);
#endif
  }
  vulkan_window& operator=(const vulkan_window&  that) = delete ;
  vulkan_window& operator=(      vulkan_window&& temp) = default;

  std::array<std::size_t, 2> drawable_size() const
  {
    std::array<std::size_t, 2> drawable_size;
    SDL_Vulkan_GetDrawableSize(native_, reinterpret_cast<int*>(&drawable_size[0]), reinterpret_cast<int*>(&drawable_size[1]));
    return drawable_size;
  }

  VkSurfaceKHR* surface()
  {
    return &surface_;
  }

private:
  VkInstance   instance_ = nullptr;
  VkSurfaceKHR surface_  = nullptr;
};
}

#endif
