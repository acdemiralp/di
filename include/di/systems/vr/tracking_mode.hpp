#ifndef DI_SYSTEMS_VR_TRACKING_MODE_HPP_
#define DI_SYSTEMS_VR_TRACKING_MODE_HPP_

#include <openvr.h>

namespace di
{
enum class tracking_mode
{
  seated       = vr::ETrackingUniverseOrigin::TrackingUniverseSeated  ,
  standing     = vr::ETrackingUniverseOrigin::TrackingUniverseStanding,
  uncalibrated = vr::ETrackingUniverseOrigin::TrackingUniverseRawAndUncalibrated,
  automatic    = 3
};
}

#endif