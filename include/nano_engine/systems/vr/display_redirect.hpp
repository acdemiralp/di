#ifndef NANO_ENGINE_SYSTEMS_VR_DISPLAY_REDIRECT_HPP_
#define NANO_ENGINE_SYSTEMS_VR_DISPLAY_REDIRECT_HPP_

#include <nano_engine/systems/vr/tracking_device.hpp>

namespace ne
{
class display_redirect : public tracking_device<tracking_device_type::display_redirect>
{
public:
  explicit display_redirect  (const std::uint32_t index) : tracking_device<tracking_device_type::display_redirect>(index)
  {
  
  }
  display_redirect           (const display_redirect&  that) = default;
  display_redirect           (      display_redirect&& temp) = default;
  virtual ~display_redirect  ()                              = default;
  display_redirect& operator=(const display_redirect&  that) = default;
  display_redirect& operator=(      display_redirect&& temp) = default;
};
}

#endif
