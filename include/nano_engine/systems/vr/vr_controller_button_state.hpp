#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_BUTTON_STATE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_BUTTON_STATE_HPP_

namespace ne
{
enum class vr_controller_button_state
{
  released = 0,
  touched  = 1,
  pressed  = 2
};
}

#endif