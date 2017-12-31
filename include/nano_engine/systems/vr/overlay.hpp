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

protected:
  overlay() : handle_(0) {}

  std::uint64_t handle_;
};
}

#endif
