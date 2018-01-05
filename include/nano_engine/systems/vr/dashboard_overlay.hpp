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
  
  // IVR Overlay - Rendering
  void                       set_tab_visible      (const bool enabled)
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_NoDashboardTab, enabled);
  }
  void                       set_gamepad_events   (const bool enabled)
  {
    vr::VROverlay()->SetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_AcceptsGamepadEvents, enabled);
  }
  
  bool                       tab_visible          () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_NoDashboardTab, &enabled);
    return enabled;
  }
  bool                       gamepad_events       () const
  {
    bool   enabled;
    vr::VROverlay()->GetOverlayFlag(id_, vr::VROverlayFlags::VROverlayFlags_AcceptsGamepadEvents, &enabled);
    return enabled;
  }

  // IVR Overlay - Dashboard
  void                       set_active           ()
  {
    vr::VROverlay()->ShowDashboard(key().c_str());
  }
  void                       set_scene_process    (const std::uint32_t process_id)
  {
    vr::VROverlay()->SetDashboardOverlaySceneProcess(id_, process_id);
  }
  
  bool                       is_active            () const
  {
    vr::VROverlay()->IsActiveDashboardOverlay(id_);
  }
  std::uint32_t              scene_process        () const
  {
    std::uint32_t process_id;
    vr::VROverlay()->GetDashboardOverlaySceneProcess(id_, &process_id);
    return process_id;
  }

  static void                set_dashboard_visible()
  {
    vr::VROverlay()->ShowDashboard(nullptr);
  }
  static bool                dashboard_visible    ()
  {
    return vr::VROverlay()->IsDashboardVisible();
  }
  static std::uint32_t       pointing_device_index()
  {
    return static_cast<std::uint32_t>(vr::VROverlay()->GetPrimaryDashboardDevice());
  }
  
protected:
  std::uint64_t thumbnail_id_;
};
}

#endif
