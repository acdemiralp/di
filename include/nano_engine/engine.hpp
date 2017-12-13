#ifndef NANO_ENGINE_ENGINE_HPP_
#define NANO_ENGINE_ENGINE_HPP_

#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

#include <nano_engine/utility/frame_timer.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class engine
{
public:
  template<typename system_type, typename... system_arguments>
  system_type* add_system   (system_arguments&&... arguments)
  {
    static_assert(std::is_base_of<system, system_type>::value, "The type does not inherit from system.");
    auto system = std::make_unique<system_type>(arguments...);
    system->engine_ = this;
    systems_.push_back(std::move(system));
    return static_cast<system_type*>(systems_.back().get());
  }
  template<typename system_type>
  system_type* get_system   ()
  {
    static_assert(std::is_base_of<system, system_type>::value, "The type does not inherit from system.");
    auto   iterator = std::find_if(systems_.begin(), systems_.end(), system_match_predicate<system_type>);
    return iterator != systems_.end() ? static_cast<system_type*>(iterator->get()) : nullptr;
  }
  template<typename system_type>
  void         remove_system()
  {
    static_assert(std::is_base_of<system, system_type>::value, "The type does not inherit from system.");
    systems_.erase(std::remove_if(systems_.begin(), systems_.end(), &system_match_predicate<system_type>), systems_.end());
  }

  void run       ()
  {
    if (is_running_) return;
    is_running_ = true;

    for (auto& system : systems_)
      system->initialize();

    frame_timer_.tick();
    while (is_running_)
    {
      frame_timer_.tick();
      for (auto& system : systems_)
        system->pre_tick ();
      for(auto& system : systems_)
        system->tick     ();
      for (auto& system : systems_)
        system->post_tick();
    }

    for (auto& system : systems_)
      system->terminate();
  }
  void stop      ()
  {
    is_running_ = false;
  }
  bool is_running() const
  {
    return is_running_;
  }

protected:
  template<typename system_type>
  static bool system_match_predicate(const std::unique_ptr<system>& iteratee)
  {
    return typeid(system_type) == typeid(*iteratee.get());
  }
  
  std::vector<std::unique_ptr<system>> systems_    ;
  frame_timer<float, std::milli>       frame_timer_;
  bool                                 is_running_ = false;
};
}

#endif
