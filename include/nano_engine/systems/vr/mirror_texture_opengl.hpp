#ifndef NANO_ENGINE_SYSTEMS_VR_MIRROR_TEXTURE_OPENGL_HPP_
#define NANO_ENGINE_SYSTEMS_VR_MIRROR_TEXTURE_OPENGL_HPP_

#include <cstdint>

#include <openvr.h>

#include <nano_engine/systems/vr/eye.hpp>

namespace ne
{
// Important Note: Accesses globals of OpenVR. Do not instantiate until vr::VR_Init has been called.
class mirror_texture_opengl
{
public:
  explicit mirror_texture_opengl  (eye eye)
  {
    vr::VRCompositor()->GetMirrorTextureGL(static_cast<vr::EVREye>(eye), &id_, reinterpret_cast<void**>(&handle_));
  }
  mirror_texture_opengl           (const mirror_texture_opengl&  that) = default;
  mirror_texture_opengl           (      mirror_texture_opengl&& temp) = default;
  virtual ~mirror_texture_opengl  ()
  {
    vr::VRCompositor()->ReleaseSharedGLTexture(id_, reinterpret_cast<void*>(handle_));
  }
  mirror_texture_opengl& operator=(const mirror_texture_opengl&  that) = default;
  mirror_texture_opengl& operator=(      mirror_texture_opengl&& temp) = default;
    
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
  std::uint32_t id_    ;
  std::uint64_t handle_;
};
}

#endif
