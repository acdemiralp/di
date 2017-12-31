#ifndef NANO_ENGINE_SYSTEMS_VR_MIRROR_TEXTURE_D3D11_HPP_
#define NANO_ENGINE_SYSTEMS_VR_MIRROR_TEXTURE_D3D11_HPP_

#include <openvr.h>

#include <nano_engine/systems/vr/eye.hpp>

class ID3D11Device;
class ID3D11ShaderResourceView;

namespace ne
{
// Important Note: Accesses globals of OpenVR. Do not instantiate until vr::VR_Init has been called.
class mirror_texture_d3d11
{
public:
  explicit mirror_texture_d3d11  (eye eye, ID3D11Device* device)
  {
    vr::VRCompositor()->GetMirrorTextureD3D11(static_cast<vr::EVREye>(eye), device, reinterpret_cast<void**>(&shader_resource_view_));
  }
  mirror_texture_d3d11           (const mirror_texture_d3d11&  that) = default;
  mirror_texture_d3d11           (      mirror_texture_d3d11&& temp) = default;
  virtual ~mirror_texture_d3d11  ()
  {
    vr::VRCompositor()->ReleaseMirrorTextureD3D11(shader_resource_view_);
  }
  mirror_texture_d3d11& operator=(const mirror_texture_d3d11&  that) = default;
  mirror_texture_d3d11& operator=(      mirror_texture_d3d11&& temp) = default;
  
  ID3D11ShaderResourceView* shader_resource_view() const
  {
    return shader_resource_view_;
  }

protected:
  ID3D11ShaderResourceView* shader_resource_view_ = nullptr;
};
}

#endif
