#ifndef NANO_ENGINE_SYSTEMS_VR_HMD_HPP_
#define NANO_ENGINE_SYSTEMS_VR_HMD_HPP_

#include <nano_engine/systems/vr/tracking_device.hpp>

namespace ne
{
class hmd : public tracking_device<tracking_device_type::hmd>
{
public:
  explicit hmd  (const std::uint32_t index = 0u) : tracking_device<tracking_device_type::hmd>(index)
  {
  
  }
  hmd           (const hmd&  that) = default;
  hmd           (      hmd&& temp) = default;
  virtual ~hmd  ()                 = default;
  hmd& operator=(const hmd&  that) = default;
  hmd& operator=(      hmd&& temp) = default;
};
}

#endif
