#ifndef DI_SYSTEMS_VR_TRACKING_DEVICE_STATE_HPP_
#define DI_SYSTEMS_VR_TRACKING_DEVICE_STATE_HPP_

#include <openvr.h>

namespace di
{
enum class tracking_device_state
{
  uninitialized            = vr::TrackingResult_Uninitialized         ,
  calibration              = vr::TrackingResult_Calibrating_InProgress,
  calibration_out_of_range = vr::TrackingResult_Calibrating_OutOfRange,
  running                  = vr::TrackingResult_Running_OK            ,
  running_out_of_range     = vr::TrackingResult_Running_OutOfRange
};
}

#endif