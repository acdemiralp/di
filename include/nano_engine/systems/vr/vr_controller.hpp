#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_HPP_

#include <openvr.h>

#include <nano_engine/systems/vr/tracking_device.hpp>

namespace ne
{
class vr_controller : public tracking_device<tracking_device_type::controller>
{
public:
  vr_controller           ()                           = default;
  vr_controller           (const vr_controller&  that) = default;
  vr_controller           (      vr_controller&& temp) = default;
  virtual ~vr_controller  ()                           = default;
  vr_controller& operator=(const vr_controller&  that) = default;
  vr_controller& operator=(      vr_controller&& temp) = default;

protected:
  
};
}

#endif