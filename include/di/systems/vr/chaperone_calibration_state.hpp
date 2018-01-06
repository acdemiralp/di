#ifndef DI_SYSTEMS_VR_CHAPERONE_CALIBRATION_STATE_HPP_
#define DI_SYSTEMS_VR_CHAPERONE_CALIBRATION_STATE_HPP_

#include <openvr.h>

namespace di
{
enum class chaperone_calibration_state
{
  ok                             = vr::ChaperoneCalibrationState_OK                             ,
  warning                        = vr::ChaperoneCalibrationState_Warning                        ,
  warning_base_station_moved     = vr::ChaperoneCalibrationState_Warning_BaseStationMayHaveMoved,
  warning_base_station_removed   = vr::ChaperoneCalibrationState_Warning_BaseStationRemoved     ,
  warning_invalid_seated_bounds  = vr::ChaperoneCalibrationState_Warning_SeatedBoundsInvalid    ,
  error                          = vr::ChaperoneCalibrationState_Error                          ,
  error_uninitialized            = vr::ChaperoneCalibrationState_Error_BaseStationUninitialized ,
  error_base_station_conflict    = vr::ChaperoneCalibrationState_Error_BaseStationConflict      ,
  error_invalid_play_area        = vr::ChaperoneCalibrationState_Error_PlayAreaInvalid          ,
  error_invalid_collision_bounds = vr::ChaperoneCalibrationState_Error_CollisionBoundsInvalid
};
}

#endif
