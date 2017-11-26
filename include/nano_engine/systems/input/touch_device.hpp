#ifndef NANO_ENGINE_SYSTEMS_INPUT_TOUCH_DEVICE_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_TOUCH_DEVICE_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <SDL2/SDL_gesture.h>
#include <SDL2/SDL_touch.h>

#include <nano_engine/systems/input/finger.hpp>

namespace ne
{
struct touch_device
{
  std::vector<finger> fingers          () const
  {
    std::vector<finger> fingers(static_cast<std::size_t>(SDL_GetNumTouchFingers(id)));
    for(auto i = 0; i < fingers.size(); ++i)
    {
      auto native_finger = SDL_GetTouchFinger(id, i);
      fingers[i] = {static_cast<std::size_t>(native_finger->id), {native_finger->x, native_finger->y}, native_finger->pressure};
    }
    return fingers;
  }
  void                record_gesture   () const
  {
    SDL_RecordGesture(id);
  }
  void                save_gesture     (const std::string& filename) const
  {
    auto stream = SDL_RWFromFile(filename.c_str(), "wb");
    SDL_SaveDollarTemplate(id, stream);
    SDL_FreeRW(stream);
  }
  void                load_gestures    (const std::string& filename)
  {
    auto stream = SDL_RWFromFile(filename.c_str(), "rb");
    SDL_LoadDollarTemplates(id, stream);
    SDL_FreeRW(stream);
  }
  
  static void         save_all_gestures(const std::string& filename)
  {
    auto stream = SDL_RWFromFile(filename.c_str(), "wb");
    SDL_SaveAllDollarTemplates(stream);
    SDL_FreeRW(stream);
  }

  std::int64_t id;  
};

inline std::vector<touch_device> touch_devices()
{
  std::vector<touch_device> touch_devices(static_cast<std::size_t>(SDL_GetNumTouchDevices()));
  for (auto i = 0; i < touch_devices.size(); ++i)
    touch_devices[i].id = SDL_GetTouchDevice(i);
  return touch_devices;
}
}

#endif
