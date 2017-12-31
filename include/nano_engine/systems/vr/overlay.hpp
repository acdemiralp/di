#ifndef NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_OVERLAY_HPP_

#include <cstdint>
#include <string>

#include <openvr.h>

namespace ne
{
class overlay
{
public:
  explicit overlay  (const std::string& key, const std::string& name)
  {
    vr::VROverlay()->CreateOverlay(key.c_str(), name.c_str(), &handle_);
  }
  overlay           (const overlay&  that) = default;
  overlay           (      overlay&& temp) = default;
  virtual ~overlay  ()
  {
    vr::VROverlay()->DestroyOverlay(handle_);
  }
  overlay& operator=(const overlay&  that) = default;
  overlay& operator=(      overlay&& temp) = default;
  
  std::string    key             ()                        const
  {
    char key[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayKey(handle_, key, vr::k_unMaxPropertyStringSize);
    return std::string(key);
  }
                 
  void           set_name        (const std::string& name)
  {
    vr::VROverlay()->SetOverlayName(handle_, name.c_str());
  }
  std::string    name            ()                        const
  {
    char name[vr::k_unMaxPropertyStringSize];
    vr::VROverlay()->GetOverlayName(handle_, name, vr::k_unMaxPropertyStringSize);
    return std::string(name);
  }
            
  void           set_visible     (bool visible)
  {
    visible ? vr::VROverlay()->ShowOverlay(handle_) : vr::VROverlay()->HideOverlay(handle_);
  }
  bool           visible         ()                        const
  {
    return vr::VROverlay()->IsOverlayVisible(handle_);
  }

  void           set_high_quality()
  {
    vr::VROverlay()->SetHighQualityOverlay(handle_);
  }
  bool           is_high_quality ()                        const
  {
    return vr::VROverlay()->GetHighQualityOverlay() == handle_;
  }
  
  static overlay find            (const std::string& key)
  {
    std::uint64_t handle;
    vr::VROverlay()->FindOverlay(key.c_str(), &handle);
    return overlay(handle);
  }

protected:
  explicit overlay(const std::uint64_t& handle = 0) : handle_(handle)
  {

  }
  
  std::uint64_t handle_;
};
}

#endif
