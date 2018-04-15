#ifndef DI_SYSTEMS_VR_SUBMIT_FLAGS_HPP_
#define DI_SYSTEMS_VR_SUBMIT_FLAGS_HPP_

#include <openvr.h>

#include <di/utility/bitset_enum.hpp>

namespace di
{
enum class submit_flags
{
  none                    = vr::Submit_Default                     ,
  lens_distortion_applied = vr::Submit_LensDistortionAlreadyApplied,
  opengl_render_buffer    = vr::Submit_GlRenderBuffer              ,
  texture_with_pose       = vr::Submit_TextureWithPose             ,
  texture_with_depth      = vr::Submit_TextureWithDepth
};
}

template<>
struct is_bitset_enum<di::submit_flags>
{
  static const bool enable = true;
};

#endif