#ifndef NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/signals2.hpp>
#include <SDL2/SDL_joystick.h>

#include <nano_engine/systems/input/joystick_hat_state.hpp>
#include <nano_engine/systems/input/joystick_power_level.hpp>

namespace ne
{
class game_controller;

class joystick
{
public:
  explicit joystick  (const std::size_t& index) : native_(SDL_JoystickOpen(static_cast<int>(index))), managed_(true)
  {
    if (!native_)
      throw std::runtime_error("Failed to create SDL joystick. SDL Error: " + std::string(SDL_GetError()));
  }
  joystick           (const joystick&  that) = delete ;
  joystick           (      joystick&& temp) = default;
  ~joystick          ()
  {
    if(managed_)
      SDL_JoystickClose(native_);
  }
  joystick& operator=(const joystick&  that) = delete ;
  joystick& operator=(      joystick&& temp) = default;

  std::string                             name          () const
  {
    return std::string(SDL_JoystickName(native_));
  }
  std::string                             guid          () const
  {
    char native_guid[128];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(native_), native_guid, 128);
    return std::string(native_guid);
  }
  bool                                    attached      () const
  {
    return SDL_JoystickGetAttached(native_) != 0;
  }
  joystick_power_level                    power_level   () const
  {
    return static_cast<joystick_power_level>(SDL_JoystickCurrentPowerLevel(native_));
  }
  
  std::vector<float>                      sticks        () const
  {
    std::vector<float> sticks(SDL_JoystickNumAxes(native_));
    for(auto i = 0; i < sticks.size(); ++i)
      sticks[i] = static_cast<float>(SDL_JoystickGetAxis(native_, static_cast<int>(i))) / 32768.0F;
    return sticks;
  }
  std::vector<bool>                       buttons       () const
  {
    std::vector<bool> buttons(SDL_JoystickNumButtons(native_));
    for(auto i = 0; i < buttons.size(); ++i)
      buttons[i] = SDL_JoystickGetButton(native_, static_cast<int>(i)) != 0;
    return buttons;
  }
  std::vector<joystick_hat_state>         hats          () const
  {
    std::vector<joystick_hat_state> hats(SDL_JoystickNumHats(native_));
    for(auto i = 0; i < hats.size(); ++i)
      hats[i] = static_cast<joystick_hat_state>(SDL_JoystickGetHat(native_, static_cast<int>(i)));
    return hats;
  }
  std::vector<std::array<std::size_t, 2>> trackballs    () const
  {
    std::vector<std::array<std::size_t, 2>> trackballs(SDL_JoystickNumBalls(native_));
    for (auto i = 0; i < trackballs.size(); ++i)
      SDL_JoystickGetBall(native_, static_cast<int>(i), reinterpret_cast<int*>(&trackballs[i][0]), reinterpret_cast<int*>(&trackballs[i][1]));
    return trackballs;
  }
  
  SDL_Joystick*                           native        () const
  {
    return native_;
  }
  std::uint32_t                           native_id     () const
  {
    return static_cast<std::uint32_t>(SDL_JoystickInstanceID(native_));
  }

  boost::signals2::signal<void(std::size_t, float)>                      on_stick_motion    ;
  boost::signals2::signal<void(std::size_t)>                             on_button_press    ;
  boost::signals2::signal<void(std::size_t)>                             on_button_release  ;
  boost::signals2::signal<void(std::size_t, joystick_hat_state)>         on_hat_motion      ;
  boost::signals2::signal<void(std::size_t, std::array<std::size_t, 2>)> on_trackball_motion;

private:
  friend game_controller;

  explicit joystick(SDL_Joystick* native) : native_(native), managed_(false)
  {
    
  }

  SDL_Joystick* native_ ;
  bool          managed_;
};
}

#endif
