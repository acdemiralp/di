#ifndef NANO_ENGINE_UTILITY_FRAME_TIMER_HPP_
#define NANO_ENGINE_UTILITY_FRAME_TIMER_HPP_

#include <chrono>

namespace ne
{
// Frame timer keeps track of time and the delta between two consecutive calls to tick().
template<typename representation = double, typename precision = std::milli>
class frame_timer final
{
public:
  using clock      = std::chrono::high_resolution_clock;
  using duration   = std::chrono::duration<representation, precision>;
  using time_point = std::chrono::time_point<clock, duration>;

  frame_timer           () : time_(clock::now()) { }
  frame_timer           (const frame_timer&  that) = default;
  frame_timer           (      frame_timer&& temp) = default;
 ~frame_timer           ()                         = default;
  frame_timer& operator=(const frame_timer&  that) = default;
  frame_timer& operator=(      frame_timer&& temp) = default;

  void tick()
  {
    auto time   = clock::now();
    delta_time_ = time - time_;
    time_       = time;
  }

  duration   delta_time() const
  {
    return delta_time_;
  }
  time_point time      () const
  {
    return time_;
  }

private:
  duration   delta_time_;
  time_point time_      ;
};
}

#endif
