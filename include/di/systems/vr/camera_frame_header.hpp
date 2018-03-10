#ifndef DI_SYSTEMS_VR_CAMERA_FRAME_HEADER_HPP_
#define DI_SYSTEMS_VR_CAMERA_FRAME_HEADER_HPP_

#include <array>
#include <cstddef>

#include <di/systems/vr/camera_frame_type.hpp>

namespace di
{
struct camera_frame_header
{
  camera_frame_type          type           ;
  std::array<std::size_t, 2> size           ;
  std::size_t                bytes_per_pixel;
  std::size_t                index          ;
};
}

#endif
