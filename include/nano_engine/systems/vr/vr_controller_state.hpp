#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_STATE_HPP_

#include <cstdint>

#include <openvr.h>

#include <nano_engine/systems/vr/vr_controller_axis.hpp>

namespace ne
{
struct vr_controller_state
{
  std::uint32_t      packet_number ;
  std::uint64_t      button_pressed;
  std::uint64_t      button_touched;
  vr_controller_axis axis[5];
};
}

#endif