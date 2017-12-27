#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SCREENSHOT_TYPE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SCREENSHOT_TYPE_HPP_

#include <openvr.h>

namespace ne
{
enum class vr_screenshot_type
{
  none            = vr::VRScreenshotType_None          ,
  mono            = vr::VRScreenshotType_Mono          ,
  stereo          = vr::VRScreenshotType_Stereo        ,
  cubemap         = vr::VRScreenshotType_Cubemap       ,
  panorama        = vr::VRScreenshotType_MonoPanorama  ,
  stereo_panorama = vr::VRScreenshotType_StereoPanorama 
};
}

#endif