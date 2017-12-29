#ifndef NANO_ENGINE_SYSTEMS_VR_CAMERA_FRAME_TYPE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_CAMERA_FRAME_TYPE_HPP_

#include <openvr.h>

namespace ne
{
enum class camera_frame_type
{
  distorted           = vr::VRTrackedCameraFrameType_Distorted         ,
  undistorted         = vr::VRTrackedCameraFrameType_Undistorted       ,
  maximum_undistorted = vr::VRTrackedCameraFrameType_MaximumUndistorted
};
}

#endif