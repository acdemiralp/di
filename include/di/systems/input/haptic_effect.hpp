#ifndef DI_SYSTEMS_INPUT_HAPTIC_EFFECT_HPP_
#define DI_SYSTEMS_INPUT_HAPTIC_EFFECT_HPP_

#include <cstddef>
#include <stdexcept>
#include <utility>

#include <SDL2/SDL_haptic.h>

#include <di/systems/input/haptic_effect_description.hpp>

namespace di
{
class haptic_effect
{
public:
  haptic_effect           (SDL_Haptic* owner, const haptic_effect_description& description) : owner_(owner)
  {
    auto native = description.native();
    if (SDL_HapticEffectSupported(owner_, &native) == 0)
      throw std::runtime_error("Failed to create SDL haptic effect: Unsupported by the device.");
    index_ = static_cast<std::size_t>(SDL_HapticNewEffect(owner_, &native));
  }
  haptic_effect           (const haptic_effect&  that) = delete ;
  haptic_effect           (      haptic_effect&& temp) noexcept : owner_(std::move(temp.owner_)), index_(std::move(temp.index_))
  {
    temp.owner_ = nullptr;
  }
  ~haptic_effect          ()
  {
    if(owner_)
      SDL_HapticDestroyEffect(owner_, static_cast<int>(index_));
  }
  haptic_effect& operator=(const haptic_effect&  that) = delete ;
  haptic_effect& operator=(      haptic_effect&& temp) noexcept
  {
    if (this != &temp)
    {
      owner_ = std::move(temp.owner_);
      index_ = std::move(temp.index_);

      temp.owner_ = nullptr;
    }
    return *this;
  }
  
  void update    (const haptic_effect_description& description) const
  {
    auto native = description.native();
    if (SDL_HapticEffectSupported(owner_, &native) == 0)
      throw std::runtime_error("Failed to update SDL haptic effect: Unsupported by the device."); 
    SDL_HapticUpdateEffect(owner_, static_cast<int>(index_), &native);
  }
  bool is_running()                                             const
  {
    return SDL_HapticGetEffectStatus(owner_, static_cast<int>(index_)) == 1;
  }
  void run       (const std::size_t repeat = 1)                 const
  {
    SDL_HapticRunEffect(owner_, static_cast<int>(index_), static_cast<unsigned>(repeat));
  }
  void stop      ()                                             const
  {
    SDL_HapticStopEffect(owner_, static_cast<int>(index_));
  }

protected:
  SDL_Haptic* owner_ = nullptr;
  std::size_t index_ = 0;
};
}

#endif
