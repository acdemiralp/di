#ifndef DI_SYSTEMS_VR_CAMERA_FRAME_TYPE_HPP_
#define DI_SYSTEMS_VR_CAMERA_FRAME_TYPE_HPP_

#include <openvr.h>

namespace di
{
enum class camera_frame_type
{
  distorted           = vr::VRTrackedCameraFrameType_Distorted         ,
  undistorted         = vr::VRTrackedCameraFrameType_Undistorted       ,
  maximum_undistorted = vr::VRTrackedCameraFrameType_MaximumUndistorted
};
}

#endif