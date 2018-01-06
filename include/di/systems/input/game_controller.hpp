#ifndef DI_SYSTEMS_INPUT_GAME_CONTROLLER_HPP_
#define DI_SYSTEMS_INPUT_GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/signals2.hpp>
#include <SDL2/SDL_gamecontroller.h>

#include <di/systems/input/game_controller_axis.hpp>
#include <di/systems/input/game_controller_button.hpp>
#include <di/systems/input/game_controller_default_mappings.hpp>
#include <di/systems/input/joystick.hpp>

namespace di
{
class game_controller
{
public:
  explicit game_controller  (const std::size_t& index) : native_(SDL_GameControllerOpen(static_cast<int>(index))), underlying_(SDL_GameControllerGetJoystick(native_))
  {
    if(!native_)
      throw std::runtime_error("Failed to create SDL game controller. SDL Error: " + std::string(SDL_GetError()));
  }
  game_controller           (const game_controller&  that) = delete ;
  game_controller           (      game_controller&& temp) noexcept
  : on_axis_motion   (std::move(temp.on_axis_motion   ))
  , on_button_press  (std::move(temp.on_button_press  ))
  , on_button_release(std::move(temp.on_button_release))
  , on_remap         (std::move(temp.on_remap         ))
  , on_close         (std::move(temp.on_close         ))
  , native_          (std::move(temp.native_          ))
  , underlying_      (std::move(temp.underlying_      ))
  {
    temp.native_ = nullptr;
  }
  ~game_controller          ()
  {
    if(native_)
      SDL_GameControllerClose(native_);
  }
  game_controller& operator=(const game_controller&  that) = delete ;
  game_controller& operator=(      game_controller&& temp) noexcept
  {
    if (this != &temp)
    {
      on_axis_motion    = std::move(temp.on_axis_motion   );
      on_button_press   = std::move(temp.on_button_press  );
      on_button_release = std::move(temp.on_button_release);
      on_remap          = std::move(temp.on_remap         );
      on_close          = std::move(temp.on_close         );
      native_           = std::move(temp.native_          );
      underlying_       = std::move(temp.underlying_      );

      temp.native_ = nullptr;
    }
    return *this;
  }
  
  std::string          name                () const
  {
    return SDL_GameControllerName(native_);
  }          
  std::string          uuid                () const
  {
    return underlying_.uuid();
  }
  std::uint32_t        instance_id         () const
  {
    return underlying_.instance_id();
  }
  std::uint32_t        product             () const
  {      
    return static_cast<std::uint32_t>(SDL_GameControllerGetProduct(native_));
  }
  std::uint32_t        product_version     () const
  {
    return static_cast<std::uint32_t>(SDL_GameControllerGetProductVersion(native_));
  }
  std::uint32_t        vendor              () const
  {
    return static_cast<std::uint32_t>(SDL_GameControllerGetVendor(native_));
  }
                                           
  joystick_power_level power_level         () const
  {
    return static_cast<joystick_power_level>(underlying_.power_level());
  }
  bool                 attached            () const
  {
    return SDL_GameControllerGetAttached(native_) != 0;
  }
                                           
  float                axis                (game_controller_axis   axis  ) const
  {
    return static_cast<float>(SDL_GameControllerGetAxis(native_, static_cast<SDL_GameControllerAxis>(axis))) / 32768.0F;
  }
  bool                 button              (game_controller_button button) const
  {
    return SDL_GameControllerGetButton(native_, static_cast<SDL_GameControllerButton>(button)) != 0u;
  }

  std::string          mapping             () const
  {
    const auto mapping_native = SDL_GameControllerMapping(native_);
    std::string mapping(mapping_native);
    SDL_free(mapping_native);
    return mapping;
  }
  void                 set_mapping         (const std::string& name, const std::string& mapping)
  {
    SDL_GameControllerAddMapping(std::string(joystick(SDL_GameControllerGetJoystick(native_)).uuid() + "," + name + "," + mapping).c_str());
  }
  
  SDL_GameController*  native              () const
  {
    return native_;
  }
  const joystick*      underlying          () const
  {
    return &underlying_;
  }

  static void          set_default_mappings()
  {
    SDL_GameControllerAddMappingsFromRW(SDL_RWFromConstMem(reinterpret_cast<const void*>(game_controller_default_mappings.data()), static_cast<int>(game_controller_default_mappings.size())), 1);
  }  
  static void          update_all          ()
  {
    SDL_GameControllerUpdate();
  }

  boost::signals2::signal<void(game_controller_axis, float)> on_axis_motion   ;
  boost::signals2::signal<void(game_controller_button)>      on_button_press  ;
  boost::signals2::signal<void(game_controller_button)>      on_button_release;
  boost::signals2::signal<void()>                            on_remap         ;
  boost::signals2::signal<void()>                            on_close         ;

private:
  SDL_GameController* native_    ;
  joystick            underlying_; // Required due to several missing functions in the SDL game controller.
};
}

#endif
