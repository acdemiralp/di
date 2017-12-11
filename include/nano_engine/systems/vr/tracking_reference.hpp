#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_REFERENCE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_REFERENCE_HPP_

#include <nano_engine/systems/vr/tracking_device.hpp>

namespace ne
{
class tracking_reference : public tracking_device<tracking_device_type::tracking_reference>
{
public:
  explicit tracking_reference  (const std::uint32_t& index) : tracking_device<tracking_device_type::tracking_reference>(index)
  {
  
  }
  tracking_reference           (const tracking_reference&  that) = default;
  tracking_reference           (      tracking_reference&& temp) = default;
  virtual ~tracking_reference  ()                                = default;
  tracking_reference& operator=(const tracking_reference&  that) = default;
  tracking_reference& operator=(      tracking_reference&& temp) = default;
};
}

#endif
