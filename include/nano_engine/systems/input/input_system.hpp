#ifndef NANO_ENGINE_SYSTEMS_INPUT_INPUT_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_INPUT_HPP_

#include <array>
#include <iostream>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

#include <nano_engine/engine.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class input_system : public system
{
public:
  input_system           ()
  {
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0)
      throw std::runtime_error("Failed to initialize SDL Events subsystem. Error: " + std::string(SDL_GetError()));
  }
  input_system           (const input_system&  that) = default;
  input_system           (      input_system&& temp) = default;
  virtual ~input_system  ()
  {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
  }
  input_system& operator=(const input_system&  that) = default;
  input_system& operator=(      input_system&& temp) = default;

  boost::signals2::signal<void()> on_quit;

protected:
  void initialize() override
  {
    on_quit.connect(std::bind(&engine::stop, engine_));
  }
  void tick      () override
  {
    std::array<SDL_Event, 128> events;
    std::size_t                count ;    
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), events.size(), SDL_GETEVENT, SDL_QUIT   , SDL_QUIT        ), count > 0)
    {
      for (std::size_t i = 0; i < count; ++i)
        on_quit();
    }
    while (SDL_PumpEvents(), count = SDL_PeepEvents(events.data(), events.size(), SDL_GETEVENT, SDL_KEYDOWN, SDL_DROPCOMPLETE), count > 0)
    {
      for(std::size_t i = 0; i < count; ++i)
      {
        auto& event = events[i];

        if      (event.type == SDL_KEYDOWN                 ) {}
        else if (event.type == SDL_KEYUP                   ) {}
        else if (event.type == SDL_TEXTEDITING             ) {}
        else if (event.type == SDL_TEXTINPUT               ) {}
        else if (event.type == SDL_KEYMAPCHANGED           ) {}
        
        else if (event.type == SDL_MOUSEMOTION             ) {}
        else if (event.type == SDL_MOUSEBUTTONDOWN         ) {}
        else if (event.type == SDL_MOUSEBUTTONUP           ) {}
        else if (event.type == SDL_MOUSEWHEEL              ) {}
        
        else if (event.type == SDL_JOYAXISMOTION           ) {}
        else if (event.type == SDL_JOYBALLMOTION           ) {}
        else if (event.type == SDL_JOYHATMOTION            ) {}
        else if (event.type == SDL_JOYBUTTONDOWN           ) {}
        else if (event.type == SDL_JOYBUTTONUP             ) {}
        else if (event.type == SDL_JOYDEVICEADDED          ) {}
        else if (event.type == SDL_JOYDEVICEREMOVED        ) {}
        
        else if (event.type == SDL_CONTROLLERAXISMOTION    ) {}
        else if (event.type == SDL_CONTROLLERBUTTONDOWN    ) {}
        else if (event.type == SDL_CONTROLLERBUTTONUP      ) {}
        else if (event.type == SDL_CONTROLLERDEVICEADDED   ) {}
        else if (event.type == SDL_CONTROLLERDEVICEREMOVED ) {}
        else if (event.type == SDL_CONTROLLERDEVICEREMAPPED) {}
        
        else if (event.type == SDL_FINGERDOWN              ) {}
        else if (event.type == SDL_FINGERUP                ) {}
        else if (event.type == SDL_FINGERMOTION            ) {}
        else if (event.type == SDL_DOLLARGESTURE           ) {}
        else if (event.type == SDL_DOLLARRECORD            ) {}
        else if (event.type == SDL_MULTIGESTURE            ) {}
        
        else if (event.type == SDL_CLIPBOARDUPDATE         ) {}
        
        else if (event.type == SDL_DROPFILE                ) {}
        else if (event.type == SDL_DROPTEXT                ) {}
        else if (event.type == SDL_DROPBEGIN               ) {}
        else if (event.type == SDL_DROPCOMPLETE            ) {}
      }
    }
  }
};
}

#endif
