#ifndef DI_SYSTEMS_DISPLAY_VULKAN_WINDOW_HPP_
#define DI_SYSTEMS_DISPLAY_VULKAN_WINDOW_HPP_

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

#include <SDL2/SDL_vulkan.h>

#include <di/systems/display/window.hpp>

namespace di
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
  vulkan_window           (      vulkan_window&& temp) noexcept : window(std::move(temp)), instance_(std::move(temp.instance_)), surface_(std::move(temp.surface_))
  {
    temp.instance_ = nullptr;
    temp.surface_  = nullptr;
  }
  ~vulkan_window          ()
  {
#ifdef VULKAN_H_
    if(instance_ != nullptr && surface_ != nullptr)
      vkDestroySurfaceKHR(instance_, surface_, nullptr);
#endif
  }
  vulkan_window& operator=(const vulkan_window&  that) = delete ;
  vulkan_window& operator=(      vulkan_window&& temp) noexcept
  {
    if (this != &temp)
    {
      window::operator=(std::move(temp));

      instance_ = std::move(temp.instance_);
      surface_  = std::move(temp.surface_ );
      
      temp.instance_ = nullptr;
      temp.surface_  = nullptr;
    }
    return *this;
  }

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
