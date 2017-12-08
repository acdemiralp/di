#ifndef NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_BUTTON_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_CONTROLLER_BUTTON_HPP_

#include <openvr.h>

namespace ne
{
enum class vr_controller_button
{
  system           = vr::k_EButton_System          ,
  application_menu = vr::k_EButton_ApplicationMenu ,
  grip             = vr::k_EButton_Grip            ,
  left             = vr::k_EButton_DPad_Left       ,
  up               = vr::k_EButton_DPad_Up         ,
  right            = vr::k_EButton_DPad_Right      ,
  down             = vr::k_EButton_DPad_Down       ,
  a                = vr::k_EButton_A               ,
  proximity_sensor = vr::k_EButton_ProximitySensor ,
  axis_0           = vr::k_EButton_Axis0           ,
  axis_1           = vr::k_EButton_Axis1           ,
  axis_2           = vr::k_EButton_Axis2           ,
  axis_3           = vr::k_EButton_Axis3           ,
  axis_4           = vr::k_EButton_Axis4           ,
  touch_pad        = vr::k_EButton_SteamVR_Touchpad,
  trigger          = vr::k_EButton_SteamVR_Trigger ,
  dashboard_back   = vr::k_EButton_Dashboard_Back
};
}

#endif