#ifndef DI_SYSTEMS_INPUT_JOYSTICK_HPP_
#define DI_SYSTEMS_INPUT_JOYSTICK_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/signals2.hpp>
#include <SDL2/SDL_joystick.h>

#include <di/systems/input/haptic_device.hpp>
#include <di/systems/input/joystick_hat_state.hpp>
#include <di/systems/input/joystick_power_level.hpp>
#include <di/systems/input/joystick_type.hpp>

namespace di
{
class game_controller;

class joystick
{
public:
  explicit joystick  (const std::size_t& index) 
  : native_ (SDL_JoystickOpen(static_cast<int>(index)))
  , managed_(true)
  , haptics_(native_ && SDL_JoystickIsHaptic(native_) ? std::make_unique<haptic_device>(SDL_HapticOpenFromJoystick(native_)) : nullptr)
  {
    if (!native_)
      throw std::runtime_error("Failed to create SDL joystick. SDL Error: " + std::string(SDL_GetError()));
  }
  joystick           (const joystick&  that) = delete ;
  joystick           (      joystick&& temp) noexcept
  : on_axis_motion     (std::move(temp.on_axis_motion     ))
  , on_button_press    (std::move(temp.on_button_press    ))
  , on_button_release  (std::move(temp.on_button_release  ))
  , on_hat_motion      (std::move(temp.on_hat_motion      ))
  , on_trackball_motion(std::move(temp.on_trackball_motion))
  , on_close           (std::move(temp.on_close           ))
  , native_            (std::move(temp.native_            ))
  , managed_           (std::move(temp.managed_           ))
  , haptics_           (std::move(temp.haptics_           ))
  {
    temp.native_ = nullptr;
  }
  ~joystick          ()
  {
    if(native_ && managed_)
      SDL_JoystickClose(native_);
  }
  joystick& operator=(const joystick&  that) = delete ;
  joystick& operator=(      joystick&& temp) noexcept
  {
    if (this != &temp)
    {
      on_axis_motion      = std::move(temp.on_axis_motion     );
      on_button_press     = std::move(temp.on_button_press    );
      on_button_release   = std::move(temp.on_button_release  );
      on_hat_motion       = std::move(temp.on_hat_motion      );
      on_trackball_motion = std::move(temp.on_trackball_motion);
      on_close            = std::move(temp.on_close           );
      native_             = std::move(temp.native_            );
      managed_            = std::move(temp.managed_           );
      haptics_            = std::move(temp.haptics_           );

      temp.native_ = nullptr;
    }
    return *this;
  }
  
  joystick_type                           type           () const
  {
    return static_cast<joystick_type>(SDL_JoystickGetType(native_));
  }
  std::string                             name           () const
  {
    return std::string(SDL_JoystickName(native_));
  }
  std::string                             uuid           () const
  {
    char native_uuid[128];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(native_), native_uuid, 128);
    return std::string(native_uuid);
  }
  std::uint32_t                           instance_id    () const
  {
    return static_cast<std::uint32_t>(SDL_JoystickInstanceID(native_));
  }
  std::uint32_t                           product        () const
  {
    return static_cast<std::uint32_t>(SDL_JoystickGetProduct(native_));
  }
  std::uint32_t                           product_version() const
  {
    return static_cast<std::uint32_t>(SDL_JoystickGetProductVersion(native_));
  }
  std::uint32_t                           vendor         () const
  {
    return static_cast<std::uint32_t>(SDL_JoystickGetVendor(native_));
  }

  joystick_power_level                    power_level    () const
  {
    return static_cast<joystick_power_level>(SDL_JoystickCurrentPowerLevel(native_));
  }
  bool                                    attached       () const
  {
    return SDL_JoystickGetAttached(native_) != 0;
  }
  
  std::vector<float>                      axes           () const
  {
    std::vector<float> axes(SDL_JoystickNumAxes(native_));
    for(auto i = 0; i < axes.size(); ++i)
      axes[i] = static_cast<float>(SDL_JoystickGetAxis(native_, static_cast<int>(i))) / 32768.0F;
    return axes;
  }
  std::vector<float>                      initial_axes   () const
  {
    std::vector<float> axes(SDL_JoystickNumAxes(native_));
    for (auto i = 0; i < axes.size(); ++i)
    {
      Sint16 state;
      SDL_JoystickGetAxisInitialState(native_, static_cast<int>(i), &state);
      axes[i] = static_cast<float>(state) / 32768.0F;
    }
    return axes;
  }
  std::vector<bool>                       buttons        () const
  {
    std::vector<bool> buttons(SDL_JoystickNumButtons(native_));
    for(auto i = 0; i < buttons.size(); ++i)
      buttons[i] = SDL_JoystickGetButton(native_, static_cast<int>(i)) != 0;
    return buttons;
  }
  std::vector<joystick_hat_state>         hats           () const
  {
    std::vector<joystick_hat_state> hats(SDL_JoystickNumHats(native_));
    for(auto i = 0; i < hats.size(); ++i)
      hats[i] = static_cast<joystick_hat_state>(SDL_JoystickGetHat(native_, static_cast<int>(i)));
    return hats;
  }
  std::vector<std::array<std::size_t, 2>> trackballs     () const
  {
    std::vector<std::array<std::size_t, 2>> trackballs(SDL_JoystickNumBalls(native_));
    for (auto i = 0; i < trackballs.size(); ++i)
      SDL_JoystickGetBall(native_, static_cast<int>(i), reinterpret_cast<int*>(&trackballs[i][0]), reinterpret_cast<int*>(&trackballs[i][1]));
    return trackballs;
  }
  
  haptic_device*                          haptics        () const
  {
    return haptics_.get();
  }

  SDL_Joystick*                           native         () const
  {
    return native_;
  }
  
  static void                             update_all     ()
  {
    SDL_JoystickUpdate();
  }
  static void                             set_global_lock(bool lock)
  {
    lock ? SDL_LockJoysticks() : SDL_UnlockJoysticks();
  }

  boost::signals2::signal<void(std::size_t, float)>                      on_axis_motion     ;
  boost::signals2::signal<void(std::size_t)>                             on_button_press    ;
  boost::signals2::signal<void(std::size_t)>                             on_button_release  ;
  boost::signals2::signal<void(std::size_t, joystick_hat_state)>         on_hat_motion      ;
  boost::signals2::signal<void(std::size_t, std::array<std::size_t, 2>)> on_trackball_motion;
  boost::signals2::signal<void()>                                        on_close           ;

protected:
  friend game_controller;

  explicit joystick(SDL_Joystick* native) 
  : native_ (native)
  , managed_(false)
  , haptics_(native_ && SDL_JoystickIsHaptic(native_) ? std::make_unique<haptic_device>(SDL_HapticOpenFromJoystick(native_)) : nullptr)
  {
    if (!native_)
      throw std::runtime_error("Failed to create SDL joystick. Invalid pointer.");
  }

  SDL_Joystick*                  native_ ;
  bool                           managed_;
  std::unique_ptr<haptic_device> haptics_;
};
}

#endif
