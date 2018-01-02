#ifndef NANO_ENGINE_SYSTEMS_VR_MIRROR_TEXTURE_OPENGL_HPP_
#define NANO_ENGINE_SYSTEMS_VR_MIRROR_TEXTURE_OPENGL_HPP_

#include <cstdint>
#include <limits>
#include <utility>

#include <openvr.h>

#include <nano_engine/systems/vr/eye.hpp>

#undef min
#undef max

namespace ne
{
class mirror_texture_opengl
{
public:
  explicit mirror_texture_opengl  (eye eye)
  {
    vr::VRCompositor()->GetMirrorTextureGL(static_cast<vr::EVREye>(eye), &id_, reinterpret_cast<void**>(&handle_));
  }
  mirror_texture_opengl           (const mirror_texture_opengl&  that) = delete ;
  mirror_texture_opengl           (      mirror_texture_opengl&& temp) noexcept : id_(std::move(temp.id_)), handle_(std::move(temp.handle_))
  {
    temp.id_ = invalid_id;
  }
  virtual ~mirror_texture_opengl  ()
  {
    if(id_ != invalid_id)
      vr::VRCompositor()->ReleaseSharedGLTexture(id_, reinterpret_cast<void*>(handle_));
  }
  mirror_texture_opengl& operator=(const mirror_texture_opengl&  that) = delete ;
  mirror_texture_opengl& operator=(      mirror_texture_opengl&& temp) noexcept
  {
    if (this != &temp)
    {
      id_     = std::move(temp.id_    );
      handle_ = std::move(temp.handle_);

      temp.id_ = invalid_id;
    }
    return *this;
  }
    
  void          lock  () const
  {
    vr::VRCompositor()->LockGLSharedTextureForAccess  (reinterpret_cast<void*>(handle_));
  }
  void          unlock() const
  {
    vr::VRCompositor()->UnlockGLSharedTextureForAccess(reinterpret_cast<void*>(handle_));
  }

  std::uint32_t id    () const
  {
    return id_;
  }
  std::uint64_t handle() const
  {
    return handle_;
  }

protected:
  static const std::uint32_t invalid_id = std::numeric_limits<std::uint32_t>::max();

  std::uint32_t id_    ;
  std::uint64_t handle_;
};
}

#endif
