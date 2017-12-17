#ifndef NANO_ENGINE_SYSTEMS_VR_SUBMIT_FLAGS_HPP_
#define NANO_ENGINE_SYSTEMS_VR_SUBMIT_FLAGS_HPP_

#include <openvr.h>

#include <nano_engine/utility/bitset_enum.hpp>

namespace ne
{
enum class submit_flags
{
  none                    = vr::Submit_Default                     ,
  lens_distortion_applied = vr::Submit_LensDistortionAlreadyApplied,
  opengl_render_buffer    = vr::Submit_GlRenderBuffer              ,
  texture_with_pose       = vr::Submit_TextureWithPose
};
}

template<>
struct is_bitset_enum<ne::submit_flags>
{
  static const bool enable = true;
};

#endif