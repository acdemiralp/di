#ifndef NANO_ENGINE_SYSTEMS_VR_DASHBOARD_OVERLAY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_DASHBOARD_OVERLAY_HPP_

#include <cstdint>
#include <string>

#include <openvr.h>

#include <nano_engine/systems/vr/overlay.hpp>

namespace ne
{
class dashboard_overlay : public overlay
{
public:
  explicit dashboard_overlay  (const std::string& key, const std::string& name)
  {
    vr::VROverlay()->CreateDashboardOverlay(key.c_str(), name.c_str(), &handle_, &thumbnail_handle_);
  }
  dashboard_overlay           (const dashboard_overlay&  that) = default;
  dashboard_overlay           (      dashboard_overlay&& temp) = default;
  virtual ~dashboard_overlay  ()                               = default;
  dashboard_overlay& operator=(const dashboard_overlay&  that) = default;
  dashboard_overlay& operator=(      dashboard_overlay&& temp) = default;

protected:
  std::uint64_t thumbnail_handle_;
};
}

#endif
