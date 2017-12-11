#ifndef NANO_ENGINE_SYSTEMS_INPUT_HAPTIC_DEVICE_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_HAPTIC_DEVICE_HPP_

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL_haptic.h>

namespace ne
{
class haptic_effect;

class haptic_device
{
public:
  explicit haptic_device  (const std::size_t& index ) : native_(SDL_HapticOpen(static_cast<int>(index)))
  {
    if(!native_)
      throw std::runtime_error("Failed to create SDL haptic device. SDL Error: " + std::string(SDL_GetError()));
    if(SDL_HapticRumbleSupported(native_))
      SDL_HapticRumbleInit(native_);
  }
  explicit haptic_device  (SDL_Haptic*        native) : native_(native)
  {
    if (!native_)
      throw std::runtime_error("Failed to create SDL haptic device. SDL Error: " + std::string(SDL_GetError()));
    if (SDL_HapticRumbleSupported(native_))
      SDL_HapticRumbleInit(native_);
  }  
  haptic_device           (const haptic_device&  that) = delete ;
  haptic_device           (      haptic_device&& temp) = default;
  ~haptic_device          ()
  {
    SDL_HapticClose(native_);
  }
  haptic_device& operator=(const haptic_device&  that) = delete ;
  haptic_device& operator=(      haptic_device&& temp) = default;
  
  std::string                 name                        () const
  {
    return std::string(SDL_HapticName(SDL_HapticIndex(native_)));
  }
  std::size_t                 maximum_effect_count        () const
  {
    return static_cast<std::size_t>(SDL_HapticNumEffects       (native_));
  }
  std::size_t                 active_effect_count         () const
  {
    return static_cast<std::size_t>(SDL_HapticNumEffectsPlaying(native_));
  }
  std::size_t                 axis_count                  () const
  {
    return static_cast<std::size_t>(SDL_HapticNumAxes          (native_));
  }
  bool                        effect_support_constant     () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_CONSTANT) != 0;
  }
  bool                        effect_support_left_right   () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_LEFTRIGHT) != 0;
  }
  bool                        effect_support_triangle_wave() const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_TRIANGLE) != 0;
  }
  bool                        effect_support_sawtooth_up  () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_SAWTOOTHUP) != 0;
  }
  bool                        effect_support_sawtooth_down() const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_SAWTOOTHDOWN) != 0;
  }
  bool                        effect_support_ramp         () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_RAMP) != 0;
  }
  bool                        effect_support_spring       () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_SPRING) != 0;
  }
  bool                        effect_support_damper       () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_DAMPER) != 0;
  }
  bool                        effect_support_inertia      () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_INERTIA) != 0;
  }
  bool                        effect_support_friction     () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_FRICTION) != 0;
  }
  bool                        effect_support_custom       () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_CUSTOM) != 0;
  }
  bool                        has_pause                   () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_PAUSE) != 0;
  }
  bool                        has_gain                    () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_GAIN) != 0;
  }
  bool                        has_autocenter              () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_AUTOCENTER) != 0;
  }
  bool                        has_status                  () const
  {
    return (SDL_HapticQuery(native_) & SDL_HAPTIC_STATUS) != 0;
  }
  
  void                        set_enabled                 (const bool        enabled   )
  {
    enabled ? SDL_HapticUnpause(native_) : SDL_HapticPause(native_);
  }
  void                        set_gain                    (const std::size_t gain      )
  {
    SDL_HapticSetGain      (native_, static_cast<int>(gain));
  }
  void                        set_autocenter              (const std::size_t autocenter)
  {
    SDL_HapticSetAutocenter(native_, static_cast<int>(autocenter));
  }
                                                          
  void                        play_rumble                 (const float strength, const std::size_t milliseconds) const
  {
    SDL_HapticRumblePlay(native_, strength, static_cast<unsigned>(milliseconds));
  }
  void                        stop_rumble                 () const
  {
    SDL_HapticRumbleStop(native_);
  }
                
  template<typename... argument_types>
  haptic_effect*              create_haptic_effect        (argument_types&&... arguments)
  {
    effects_.emplace_back(std::make_unique<haptic_effect>(arguments...));
    return effects_.back().get();
  }
  void                        destroy_haptic_effect       (haptic_effect* effect)
  {
    effects_.erase(std::remove_if(
      effects_.begin(),
      effects_.end  (),
      [&effect] (const std::unique_ptr<haptic_effect>& iteratee)
      {
        return iteratee.get() == effect;
      }), 
      effects_.end  ());
  }
  std::vector<haptic_effect*> haptic_effects              () const
  {
    std::vector<haptic_effect*> haptic_effects(effects_.size());
    std::transform(
      effects_      .begin(),
      effects_      .end  (),
      haptic_effects.begin(),
      [ ] (const std::unique_ptr<haptic_effect>& effect)
      {
        return effect.get();
      });
    return haptic_effects;
  }
                          
  void                        stop_all_effects            () const
  {
    SDL_HapticStopAll(native_);
  }

  SDL_Haptic*                 native                      () const
  {
    return native_;
  }

protected:
  SDL_Haptic*                                 native_ ;
  std::vector<std::unique_ptr<haptic_effect>> effects_;
};
  
class haptic_effect
{
public:
  haptic_effect           (haptic_device* owner, const std::size_t index) : owner_(owner), index_(index)
  {
    
  }
  haptic_effect           (const haptic_effect&  that) = default;
  haptic_effect           (      haptic_effect&& temp) = default;
  ~haptic_effect          ()
  {
    SDL_HapticDestroyEffect(owner_->native(), static_cast<int>(index_));
  }
  haptic_effect& operator=(const haptic_effect&  that) = default;
  haptic_effect& operator=(      haptic_effect&& temp) = default;
  
  bool is_running()                             const
  {
    return SDL_HapticGetEffectStatus(owner_->native(), static_cast<int>(index_)) == 1;
  }
  void run       (const std::size_t repeat = 1) const
  {
    SDL_HapticRunEffect(owner_->native(), static_cast<int>(index_), static_cast<unsigned>(repeat));
  }
  void stop      ()                             const
  {
    SDL_HapticStopEffect(owner_->native(), static_cast<int>(index_));
  }

protected:
  haptic_device* owner_ = nullptr;
  std::size_t    index_ = 0;
};

class haptic_effect_description
{
  
};
class constant_haptic_effect_description : public haptic_effect_description
{
  std::array<std::size_t, 2> direction;
  std::size_t length;
  std::size_t delay ;
};

}

#endif
