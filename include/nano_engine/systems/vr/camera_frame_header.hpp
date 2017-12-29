#ifndef NANO_ENGINE_SYSTEMS_VR_CAMERA_FRAME_HEADER_HPP_
#define NANO_ENGINE_SYSTEMS_VR_CAMERA_FRAME_HEADER_HPP_

#include <array>
#include <cstddef>

#include <nano_engine/systems/vr/camera_frame_type.hpp>

namespace ne
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
