#ifndef NANO_ENGINE_SYSTEMS_INPUT_GAME_CONTROLLER_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>

#include <SDL2/SDL_gamecontroller.h>

#include <nano_engine/systems/input/game_controller_axis.hpp>
#include <nano_engine/systems/input/joystick.hpp>

namespace ne
{
class game_controller
{
public:
  explicit game_controller  (std::size_t index) : native_(SDL_GameControllerOpen(static_cast<int>(index)))
  {
    if(!native_)
      throw std::runtime_error("Failed to create SDL game controller. SDL Error: " + std::string(SDL_GetError()));
  }
  game_controller           (const game_controller&  that) = delete ;
  game_controller           (      game_controller&& temp) = default;
  ~game_controller          ()
  {
    SDL_GameControllerClose(native_);
  }
  game_controller& operator=(const game_controller&  that) = delete ;
  game_controller& operator=(      game_controller&& temp) = default;
  
  // type
  std::string         name           () const
  {
    return SDL_GameControllerName(native_);
  }          
  // guid
  // instance_id
  std::uint32_t       product        () const
  {      
    return static_cast<std::uint32_t>(SDL_GameControllerGetProduct(native_));
  }
  std::uint32_t       product_version() const
  {
    return static_cast<std::uint32_t>(SDL_GameControllerGetProductVersion(native_));
  }
  std::uint32_t       vendor         () const
  {
    return static_cast<std::uint32_t>(SDL_GameControllerGetVendor(native_));
  }
  
  // power_level
  bool                attached       () const
  {
    return SDL_GameControllerGetAttached(native_) != 0;
  }
   
  float               axis           (game_controller_axis axis) const
  {
    return static_cast<float>(SDL_GameControllerGetAxis(native_, static_cast<SDL_GameControllerAxis>(axis))) / 32768.0F;
  }
  
  // Mapping string keys: a, b, x, y, start, guide, back, up, left, down, right, left_shoulder, right_shoulder, left_stick, right_stick, left_x, left_y, right_x, right_y, left_trigger, right_trigger
  // Example mapping string: "a:b1,b:b2,y:b3,x:b0,start:b9,guide:b12,back:b8,dpup:h0.1,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,leftshoulder:b4,rightshoulder:b5,leftstick:b10,rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7"
  void                set_mapping    (const std::string& name, const std::string mapping)
  {
    SDL_GameControllerAddMapping(std::string(joystick(SDL_GameControllerGetJoystick(native_)).guid() + "," + name + "," + mapping).c_str());
  }

  SDL_GameController* native         () const
  {
    return native_;
  }

  static void         update_all     ()
  {
    SDL_GameControllerUpdate();
  }

private:
  SDL_GameController* native_;
};
}

#endif
