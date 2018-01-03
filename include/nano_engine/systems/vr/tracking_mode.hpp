#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_MODE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_MODE_HPP_

#include <openvr.h>

namespace ne
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