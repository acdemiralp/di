#ifndef NANO_ENGINE_SYSTEMS_VR_DASHBOARD_OVERLAY_HPP_
#define NANO_ENGINE_SYSTEMS_VR_DASHBOARD_OVERLAY_HPP_

#include <cstdint>
#include <string>
#include <utility>

#include <openvr.h>

#include <nano_engine/systems/vr/overlay.hpp>

namespace ne
{
class dashboard_overlay : public overlay
{
public:
  explicit dashboard_overlay  (const std::string& key, const std::string& name)
  {
    vr::VROverlay()->CreateDashboardOverlay(key.c_str(), name.c_str(), &id_, &thumbnail_id_);
  }
  dashboard_overlay           (const dashboard_overlay&  that) = delete ;
  dashboard_overlay           (      dashboard_overlay&& temp) noexcept : overlay(std::move(temp)), thumbnail_id_(std::move(temp.thumbnail_id_))
  {
    temp.thumbnail_id_ = invalid_id;
  }
  virtual ~dashboard_overlay  ()
  {
    if(thumbnail_id_ != invalid_id)
      vr::VROverlay()->DestroyOverlay(thumbnail_id_);
  }
  dashboard_overlay& operator=(const dashboard_overlay&  that) = delete ;
  dashboard_overlay& operator=(      dashboard_overlay&& temp) noexcept
  {
    if (this != &temp)
    {
      overlay::operator=(std::move(temp));

      thumbnail_id_ = std::move(temp.thumbnail_id_);

      temp.thumbnail_id_ = invalid_id;
    }
    return *this;
  }

  void          set_scene_process(const std::uint32_t process_id)
  {
    vr::VROverlay()->SetDashboardOverlaySceneProcess(id_, process_id);
  }

  std::uint32_t scene_process    ()                               const
  {
    std::uint32_t process_id;
    vr::VROverlay()->GetDashboardOverlaySceneProcess(id_, &process_id);
    return process_id;
  }

protected:
  std::uint64_t thumbnail_id_;
};
}

#endif
