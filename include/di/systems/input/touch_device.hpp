#ifndef DI_SYSTEMS_INPUT_TOUCH_DEVICE_HPP_
#define DI_SYSTEMS_INPUT_TOUCH_DEVICE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <boost/signals2.hpp>
#include <SDL2/SDL_gesture.h>
#include <SDL2/SDL_touch.h>

#include <di/systems/input/finger.hpp>
#include <di/systems/input/gesture.hpp>
#include <di/systems/input/multi_gesture.hpp>

namespace di
{
class input_system;

class touch_device
{
public:
  explicit touch_device  (const std::size_t& index) : id_(SDL_GetTouchDevice(static_cast<int>(index)))
  {
    if(!id_)
      throw std::runtime_error("Failed to get SDL touch device. SDL Error: " + std::string(SDL_GetError()));
  }
  touch_device           (const touch_device&  that) = default;
  touch_device           (      touch_device&& temp) = default;
  ~touch_device          ()                          = default;
  touch_device& operator=(const touch_device&  that) = default;
  touch_device& operator=(      touch_device&& temp) = default;
  
  std::vector<finger> fingers       () const
  {
    std::vector<finger> fingers(static_cast<std::size_t>(SDL_GetNumTouchFingers(id_)));
    for(auto i = 0; i < fingers.size(); ++i)
    {
      const auto native_finger = SDL_GetTouchFinger(id_, i);
      fingers[i] = {static_cast<std::size_t>(native_finger->id), {native_finger->x, native_finger->y}, native_finger->pressure};
    }
    return fingers;
  }
  void                record_gesture(const std::function<void(gesture)>& callback)
  {
    record_gesture_callback_ = callback;
    SDL_RecordGesture(id_);
  }
  static void         load_gestures (const std::string& filename)
  {
    const auto stream = SDL_RWFromFile(filename.c_str(), "rb");
    SDL_LoadDollarTemplates(-1, stream);
    SDL_FreeRW(stream);
  }
  static void         save_gestures (const std::string& filename)
  {
    const auto stream = SDL_RWFromFile(filename.c_str(), "wb");
    SDL_SaveAllDollarTemplates(stream);
    SDL_FreeRW(stream);
  }

  boost::signals2::signal<void(finger)>        on_finger_press  ;
  boost::signals2::signal<void(finger)>        on_finger_release;
  boost::signals2::signal<void(finger)>        on_finger_motion ;
  boost::signals2::signal<void(gesture)>       on_gesture       ;
  boost::signals2::signal<void(multi_gesture)> on_multi_gesture ;
  
private:
  friend input_system;

  std::int64_t id_;
  std::function<void(gesture)> record_gesture_callback_;
};
}

#endif
