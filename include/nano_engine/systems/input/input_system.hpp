#ifndef NANO_ENGINE_SYSTEMS_INPUT_INPUT_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_INPUT_HPP_

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

#include <nano_engine/systems/input/clipboard.hpp>
#include <nano_engine/systems/input/key.hpp>
#include <nano_engine/systems/input/game_controller.hpp>
#include <nano_engine/systems/input/game_controller_info.hpp>
#include <nano_engine/systems/input/haptic_device.hpp>
#include <nano_engine/systems/input/joystick.hpp>
#include <nano_engine/systems/input/joystick_info.hpp>
#include <nano_engine/systems/input/touch_device.hpp>
#include <nano_engine/engine.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class input_system : public system
{
public:
  input_system           ()
  {
    if (SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0)
      throw std::runtime_error("Failed to initialize SDL Events / Game Controller / Joystick subsystems. Error: " + std::string(SDL_GetError()));

    game_controller::set_default_mappings();

    for (auto i = 0; i < SDL_GetNumTouchDevices(); ++i)
      touch_devices_.emplace_back(std::make_unique<touch_device>(SDL_GetTouchDevice(i)));
  }
  input_system           (const input_system&  that) = delete ;
  input_system           (      input_system&& temp) = default;
  virtual ~input_system  ()
  {
    SDL_QuitSubSystem(SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  }
  input_system& operator=(const input_system&  that) = delete ;
  input_system& operator=(      input_system&& temp) = default;

  template<typename... argument_types>
  joystick*                     create_joystick        (argument_types&&... arguments)
  {
    joysticks_.emplace_back(std::make_unique<joystick>(arguments...));
    return joysticks_.back().get();
  }
  void                          destroy_joystick       (joystick* joystick)
  {
    joysticks_.erase(std::remove_if(
      joysticks_.begin(),
      joysticks_.end  (),
      [&joystick] (const std::unique_ptr<ne::joystick>& iteratee)
      {
        return iteratee.get() == joystick;
      }), 
      joysticks_.end  ());
  }
  std::vector<joystick*>        joysticks              () const
  {
    std::vector<joystick*> joysticks(joysticks_.size());
    std::transform(
      joysticks_.begin(),
      joysticks_.end  (),
      joysticks .begin(),
      [ ] (const std::unique_ptr<joystick>& joystick)
      {
        return joystick.get();
      });
    return joysticks;
  }
  
  template<typename... argument_types>
  game_controller*              create_game_controller (argument_types&&... arguments)
  {
    game_controllers_.emplace_back(std::make_unique<game_controller>(arguments...));
    return game_controllers_.back().get();
  }
  void                          destroy_game_controller(game_controller* game_controller)
  {
    game_controllers_.erase(std::remove_if(
      game_controllers_.begin(),
      game_controllers_.end  (),
      [&game_controller] (const std::unique_ptr<ne::game_controller>& iteratee)
      {
        return iteratee.get() == game_controller;
      }), 
      game_controllers_.end  ());
  }
  std::vector<game_controller*> game_controllers       () const
  {
    std::vector<game_controller*> game_controllers(game_controllers_.size());
    std::transform(
      game_controllers_.begin(),
      game_controllers_.end  (),
      game_controllers .begin(),
      [ ] (const std::unique_ptr<game_controller>& game_controller)
      {
        return game_controller.get();
      });
    return game_controllers;
  }
  
  template<typename... argument_types>
  haptic_device*                create_haptic_device   (argument_types&&... arguments)
  {
    haptic_devices_.emplace_back(std::make_unique<haptic_device>(arguments...));
    return haptic_devices_.back().get();
  }
  void                          destroy_haptic_device  (haptic_device* haptic_device)
  {
    haptic_devices_.erase(std::remove_if(
      haptic_devices_.begin(),
      haptic_devices_.end  (),
      [&haptic_device] (const std::unique_ptr<ne::haptic_device>& iteratee)
      {
        return iteratee.get() == haptic_device;
      }), 
      haptic_devices_.end  ());
  }
  std::vector<haptic_device*>   haptic_devices         () const
  {
    std::vector<haptic_device*> haptic_devices(haptic_devices_.size());
    std::transform(
      haptic_devices_.begin(),
      haptic_devices_.end  (),
      haptic_devices .begin(),
      [ ] (const std::unique_ptr<haptic_device>& haptic_device)
      {
        return haptic_device.get();
      });
    return haptic_devices;
  }

  std::vector<touch_device*>    touch_devices          () const
  {
    std::vector<touch_device*> touch_devices(touch_devices_.size());
    std::transform(
      touch_devices_.begin(),
      touch_devices_.end  (),
      touch_devices .begin(),
      [ ] (const std::unique_ptr<touch_device>& touch_device)
      {
        return touch_device.get();
      });
    return touch_devices;
  }

  static bool                   has_events             ()
  {
    return SDL_HasEvents(SDL_FIRSTEVENT, SDL_LASTEVENT) != 0;
  }
  static void                   flush_events           ()
  {
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }
  static bool                   quit_pending           ()
  {
    return SDL_QuitRequested();
  }
                                                       
  static bool                   text_input_enabled     ()
  {
    return SDL_IsTextInputActive() != 0;
  }
  static void                   set_text_input_enabled (bool enabled)
  {
    enabled ? SDL_StartTextInput() : SDL_StopTextInput();
  }
  static void                   set_text_input_area    (const std::array<std::size_t, 2>& position, const std::array<std::size_t, 2>& size)
  {
    SDL_Rect rectangle {
      static_cast<int>(position[0]), 
      static_cast<int>(position[1]), 
      static_cast<int>(size    [0]), 
      static_cast<int>(size    [1])};
    SDL_SetTextInputRect(&rectangle);
  }
  
  boost::signals2::signal<void(key)>                                   on_key_press              ;
  boost::signals2::signal<void(key)>                                   on_key_release            ;
  boost::signals2::signal<void(std::string, std::size_t, std::size_t)> on_text_edit              ;
  boost::signals2::signal<void(std::string)>                           on_text_input             ;
  boost::signals2::signal<void()>                                      on_key_layout_change      ;
  boost::signals2::signal<void(std::array<std::size_t, 2>)>            on_mouse_move             ;
  boost::signals2::signal<void(std::array<std::size_t, 2>)>            on_mouse_move_delta       ;
  boost::signals2::signal<void(std::size_t)>                           on_mouse_press            ;
  boost::signals2::signal<void(std::size_t)>                           on_mouse_release          ;
  boost::signals2::signal<void(std::array<std::size_t, 2>)>            on_mouse_wheel            ;
  boost::signals2::signal<void(joystick_info)>                         on_joystick_connect       ;
  boost::signals2::signal<void(game_controller_info)>                  on_game_controller_connect;
  boost::signals2::signal<void(std::string)>                           on_clipboard_change       ;
  boost::signals2::signal<void()>                                      on_quit                   ;

protected:
  void initialize() override
  {
    on_quit.connect(std::bind(&engine::stop, engine_));
  }
  void tick      () override
  {
    std::array<SDL_Event, 128> events;
    int                        count ;    
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), static_cast<int>(events.size()), SDL_GETEVENT, SDL_QUIT   , SDL_QUIT        ), count > 0)
    {
      for (auto i = 0; i < count; ++i)
        on_quit();
    }
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), static_cast<int>(events.size()), SDL_GETEVENT, SDL_KEYDOWN, SDL_DROPCOMPLETE), count > 0)
    {
      for(auto i = 0; i < count; ++i)
      {
        auto& event = events[i];

        if      (event.type == SDL_KEYDOWN                 ) on_key_press        (key{static_cast<key_code>(event.key.keysym.sym), static_cast<key_modifier>(event.key.keysym.mod), static_cast<scan_code>(event.key.keysym.scancode)});
        else if (event.type == SDL_KEYUP                   ) on_key_release      (key{static_cast<key_code>(event.key.keysym.sym), static_cast<key_modifier>(event.key.keysym.mod), static_cast<scan_code>(event.key.keysym.scancode)});
        else if (event.type == SDL_TEXTEDITING             ) on_text_edit        (std::string(event.edit.text), static_cast<std::size_t>(event.edit.start), static_cast<std::size_t>(event.edit.length));
        else if (event.type == SDL_TEXTINPUT               ) on_text_input       (std::string(event.text.text));
        else if (event.type == SDL_KEYMAPCHANGED           ) on_key_layout_change();
        
        else if (event.type == SDL_MOUSEMOTION             )
        {
          on_mouse_move      ({static_cast<std::size_t>(event.motion.x   ), static_cast<std::size_t>(event.motion.y   )});
          on_mouse_move_delta({static_cast<std::size_t>(event.motion.xrel), static_cast<std::size_t>(event.motion.yrel)});
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN         ) on_mouse_press  ( static_cast<std::size_t>(event.button.button));
        else if (event.type == SDL_MOUSEBUTTONUP           ) on_mouse_release( static_cast<std::size_t>(event.button.button));
        else if (event.type == SDL_MOUSEWHEEL              ) on_mouse_wheel  ({static_cast<std::size_t>(event.wheel.x), static_cast<std::size_t>(event.wheel.y)});
        
        else if (event.type == SDL_JOYAXISMOTION           )
        {
          auto joystick = std::find_if(joysticks_.begin(), joysticks_.end(), [&event] (const std::unique_ptr<ne::joystick>& iteratee) { return iteratee->instance_id() == event.jaxis.which; });
          if  (joystick == joysticks_.end()) continue;
          joystick->get()->on_axis_motion(static_cast<std::size_t>(event.jaxis.axis), static_cast<float>(event.jaxis.value) / 32768.0F);
        }
        else if (event.type == SDL_JOYBALLMOTION           ) 
        {
          auto joystick = std::find_if(joysticks_.begin(), joysticks_.end(), [&event] (const std::unique_ptr<ne::joystick>& iteratee) { return iteratee->instance_id() == event.jball.which; });
          if  (joystick == joysticks_.end()) continue;
          joystick->get()->on_trackball_motion(static_cast<std::size_t>(event.jball.ball), {static_cast<std::size_t>(event.jball.xrel), static_cast<std::size_t>(event.jball.yrel)});
        }
        else if (event.type == SDL_JOYHATMOTION            ) 
        {
          auto joystick = std::find_if(joysticks_.begin(), joysticks_.end(), [&event] (const std::unique_ptr<ne::joystick>& iteratee) { return iteratee->instance_id() == event.jhat.which; });
          if  (joystick == joysticks_.end()) continue;
          joystick->get()->on_hat_motion(static_cast<std::size_t>(event.jhat.hat), static_cast<joystick_hat_state>(event.jhat.value));
        }
        else if (event.type == SDL_JOYBUTTONDOWN           ) 
        {
          auto joystick = std::find_if(joysticks_.begin(), joysticks_.end(), [&event] (const std::unique_ptr<ne::joystick>& iteratee) { return iteratee->instance_id() == event.jbutton.which; });
          if  (joystick == joysticks_.end()) continue;
          joystick->get()->on_button_press(static_cast<std::size_t>(event.jbutton.button));
        }
        else if (event.type == SDL_JOYBUTTONUP             ) 
        {
          auto joystick = std::find_if(joysticks_.begin(), joysticks_.end(), [&event] (const std::unique_ptr<ne::joystick>& iteratee) { return iteratee->instance_id() == event.jbutton.which; });
          if  (joystick == joysticks_.end()) continue;
          joystick->get()->on_button_press(static_cast<std::size_t>(event.jbutton.button));
        }
        else if (event.type == SDL_JOYDEVICEADDED          ) on_joystick_connect(joystick_infos()[event.jdevice.which]);
        else if (event.type == SDL_JOYDEVICEREMOVED        )
        {
          auto joystick = std::find_if(joysticks_.begin(), joysticks_.end(), [&event] (const std::unique_ptr<ne::joystick>& iteratee) { return iteratee->instance_id() == event.jdevice.which; });
          if  (joystick == joysticks_.end()) continue;
          joystick->get()->on_close();
        }
        
        else if (event.type == SDL_CONTROLLERAXISMOTION    )
        {
          auto game_controller = std::find_if(game_controllers_.begin(), game_controllers_.end(), [&event] (const std::unique_ptr<ne::game_controller>& iteratee) { return iteratee->instance_id() == event.caxis.which; });
          if  (game_controller == game_controllers_.end()) continue;
          game_controller->get()->on_axis_motion(static_cast<game_controller_axis>(event.caxis.axis), static_cast<float>(event.caxis.value) / 32768.0F);
        }
        else if (event.type == SDL_CONTROLLERBUTTONDOWN    )
        {
          auto game_controller = std::find_if(game_controllers_.begin(), game_controllers_.end(), [&event] (const std::unique_ptr<ne::game_controller>& iteratee) { return iteratee->instance_id() == event.cbutton.which; });
          if  (game_controller == game_controllers_.end()) continue;
          game_controller->get()->on_button_press(static_cast<game_controller_button>(event.cbutton.button));
        }
        else if (event.type == SDL_CONTROLLERBUTTONUP      )
        {
          auto game_controller = std::find_if(game_controllers_.begin(), game_controllers_.end(), [&event] (const std::unique_ptr<ne::game_controller>& iteratee) { return iteratee->instance_id() == event.cbutton.which; });
          if  (game_controller == game_controllers_.end()) continue;
          game_controller->get()->on_button_press(static_cast<game_controller_button>(event.cbutton.button));
        }
        else if (event.type == SDL_CONTROLLERDEVICEADDED   ) on_game_controller_connect(game_controller_infos()[event.cdevice.which]);
        else if (event.type == SDL_CONTROLLERDEVICEREMOVED )
        {
          auto game_controller = std::find_if(game_controllers_.begin(), game_controllers_.end(), [&event] (const std::unique_ptr<ne::game_controller>& iteratee) { return iteratee->instance_id() == event.cdevice.which; });
          if  (game_controller == game_controllers_.end()) continue;
          game_controller->get()->on_close();
        }
        else if (event.type == SDL_CONTROLLERDEVICEREMAPPED)
        {
          auto game_controller = std::find_if(game_controllers_.begin(), game_controllers_.end(), [&event] (const std::unique_ptr<ne::game_controller>& iteratee) { return iteratee->instance_id() == event.cdevice.which; });
          if  (game_controller == game_controllers_.end()) continue;
          game_controller->get()->on_remap();
        }
        
        else if (event.type == SDL_FINGERDOWN              )
        {
          auto touch_device = std::find_if(touch_devices_.begin(), touch_devices_.end(), [&event] (const std::unique_ptr<ne::touch_device>& iteratee) { return iteratee->id_ == event.tfinger.touchId; });
          if  (touch_device == touch_devices_.end()) continue;
          touch_device->get()->on_finger_press  (finger{static_cast<std::size_t>(event.tfinger.fingerId), {event.tfinger.x , event.tfinger.y }, event.tfinger.pressure});
        }
        else if (event.type == SDL_FINGERUP                )
        {
          auto touch_device = std::find_if(touch_devices_.begin(), touch_devices_.end(), [&event] (const std::unique_ptr<ne::touch_device>& iteratee) { return iteratee->id_ == event.tfinger.touchId; });
          if  (touch_device == touch_devices_.end()) continue;
          touch_device->get()->on_finger_release(finger{static_cast<std::size_t>(event.tfinger.fingerId), {event.tfinger.x , event.tfinger.y }, event.tfinger.pressure});
        }
        else if (event.type == SDL_FINGERMOTION            )
        {
          auto touch_device = std::find_if(touch_devices_.begin(), touch_devices_.end(), [&event] (const std::unique_ptr<ne::touch_device>& iteratee) { return iteratee->id_ == event.tfinger.touchId; });
          if  (touch_device == touch_devices_.end()) continue;
          touch_device->get()->on_finger_motion (finger{static_cast<std::size_t>(event.tfinger.fingerId), {event.tfinger.dx, event.tfinger.dy}, event.tfinger.pressure});
        }
        else if (event.type == SDL_DOLLARGESTURE           )
        {
          auto touch_device = std::find_if(touch_devices_.begin(), touch_devices_.end(), [&event] (const std::unique_ptr<ne::touch_device>& iteratee) { return iteratee->id_ == event.dgesture.touchId; });
          if  (touch_device == touch_devices_.end()) continue;
          touch_device->get()->on_gesture(gesture{event.dgesture.gestureId, {event.dgesture.x, event.dgesture.y}, event.dgesture.error, static_cast<std::size_t>(event.dgesture.numFingers)});
        }
        else if (event.type == SDL_DOLLARRECORD            )
        {
          auto touch_device = std::find_if(touch_devices_.begin(), touch_devices_.end(), [&event] (const std::unique_ptr<ne::touch_device>& iteratee) { return iteratee->id_ == event.dgesture.touchId; });
          if  (touch_device == touch_devices_.end()) continue;
          touch_device->get()->record_gesture_callback_(gesture{event.dgesture.gestureId});  
        }
        else if (event.type == SDL_MULTIGESTURE            )
        {
          auto touch_device = std::find_if(touch_devices_.begin(), touch_devices_.end(), [&event] (const std::unique_ptr<ne::touch_device>& iteratee) { return iteratee->id_ == event.mgesture.touchId; });
          if  (touch_device == touch_devices_.end()) continue;
          touch_device->get()->on_multi_gesture(multi_gesture{{event.mgesture.x, event.mgesture.y}, event.mgesture.dTheta, event.mgesture.dDist, event.mgesture.numFingers});  
        }
        
        else if (event.type == SDL_CLIPBOARDUPDATE         ) on_clipboard_change(clipboard::get());
      }
    } 

    joystick::       update_all();
    game_controller::update_all();
  }

  std::vector<std::unique_ptr<joystick>>        joysticks_       ;
  std::vector<std::unique_ptr<game_controller>> game_controllers_;
  std::vector<std::unique_ptr<haptic_device>>   haptic_devices_  ;
  std::vector<std::unique_ptr<touch_device>>    touch_devices_   ;
};
}

#endif
