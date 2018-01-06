#ifndef DI_SYSTEMS_VR_VR_CONTROLLER_BUTTON_HPP_
#define DI_SYSTEMS_VR_VR_CONTROLLER_BUTTON_HPP_

#include <vector>

#include <openvr.h>

namespace di
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
  touch_pad        = vr::k_EButton_SteamVR_Touchpad, // Alias
  trigger          = vr::k_EButton_SteamVR_Trigger , // Alias
  dashboard_back   = vr::k_EButton_Dashboard_Back    // Alias
};

// Enable iterating over non-contiguous enum.
std::vector<vr_controller_button> vr_controller_buttons 
{
  vr_controller_button::system          ,
  vr_controller_button::application_menu,
  vr_controller_button::grip            ,
  vr_controller_button::left            ,
  vr_controller_button::up              ,
  vr_controller_button::right           ,
  vr_controller_button::down            ,
  vr_controller_button::a               ,
  vr_controller_button::proximity_sensor,
  vr_controller_button::axis_0          ,
  vr_controller_button::axis_1          ,
  vr_controller_button::axis_2          ,
  vr_controller_button::axis_3          ,
  vr_controller_button::axis_4          ,
  vr_controller_button::touch_pad       ,
  vr_controller_button::trigger         ,
  vr_controller_button::dashboard_back  ,
};
}

#endif