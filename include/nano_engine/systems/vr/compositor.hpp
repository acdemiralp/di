#ifndef NANO_ENGINE_SYSTEMS_VR_COMPOSITOR_HPP_
#define NANO_ENGINE_SYSTEMS_VR_COMPOSITOR_HPP_

#include <array>

#include <openvr.h>

#include <nano_engine/systems/vr/tracking_mode.hpp>

namespace ne
{
// Important Note: Accesses globals of OpenVR. Do not instantiate until vr::VR_Init has been called.
class compositor
{
public:
  compositor           () : native_(vr::VRCompositor())
  {
    
  }
  compositor           (const compositor&  that) = default;
  compositor           (      compositor&& temp) = default;
  virtual ~compositor  ()                        = default;
  compositor& operator=(const compositor&  that) = default;
  compositor& operator=(      compositor&& temp) = default;

  void                 set_paused                  (const bool pause  )
  {
    native_->SuspendRendering(pause);
  }

  void                 set_mirror_window_visible   (const bool visible)
  {
    visible ? native_->ShowMirrorWindow() : native_->HideMirrorWindow();
  }
  bool                 mirror_window_visible       ()                                                                                        const
  {
    return native_->IsMirrorWindowVisible();
  }
                                                                                                                                             
  void                 set_tracking_mode           (tracking_mode mode)
  {
    native_->SetTrackingSpace(static_cast<vr::ETrackingUniverseOrigin>(mode));
  }                                                             
  tracking_mode        tracking_mode               ()                                                                                        const
  {
    return static_cast<ne::tracking_mode>(native_->GetTrackingSpace());
  }
                                                                                                                                             
  bool                 is_fullscreen               ()                                                                                        const
  {
    return native_->IsFullscreen();
  }                                
  bool                 can_render                  ()                                                                                        const
  {
    return native_->CanRenderScene();
  }
  bool                 low_resources               ()                                                                                        const
  {
    return native_->ShouldAppRenderWithLowResources();
  }
                                                                                                                                             
  void                 to_front                    ()                                                                                        const
  {
    native_->CompositorBringToFront();
  }
  void                 to_back                     ()                                                                                        const
  {
    native_->CompositorGoToBack();
  }
  void                 dump_images                 ()                                                                                        const
  {
    native_->CompositorDumpImages();
  }
                                                                                                                                             
                                                                                                                                             
  void                 unblock                     ()                                                                                        const
  {
    native_->PostPresentHandoff();
  }
               
  void                 fade_to_color               (std::array<float, 4> color  , const float seconds = 1.0F, const bool background = false) const
  {                                                
    native_->FadeToColor(seconds, color[0], color[1], color[2], color[3], background);
  }                                                
  std::array<float, 4> fade_color                  (                                                          const bool background = false) const
  {                                                
    const auto native_color = native_->GetCurrentFadeColor(background);
    return {native_color.r, native_color.g, native_color.b, native_color.a};
  }
  void                 fade_grid                   (const bool           fade_in, const float seconds = 1.0F)                                const
  {                                                
    native_->FadeGrid(seconds, fade_in);
  }                                                
  float                fade_grid_alpha             ()                                                                                        const
  {                                                                                                                                        
    return native_->GetCurrentGridAlpha();                                                                                                 
  }                                                                                                                                        

  std::uint32_t        process_id                  ()                                                                                        const
  {
    return native_->GetCurrentSceneFocusProcess();
  }
  std::uint32_t        last_process_id             ()                                                                                        const
  {
    return native_->GetLastFrameRenderer();
  }

  void                 force_interleaved_projection(const bool enabled)
  {
    native_->ForceInterleavedReprojectionOn(enabled);
  }
  void                 force_reconnect_to_process  ()
  {
    native_->ForceReconnectProcess();
  }
  void                 force_quit                  ()
  {
    return native_->CompositorQuit();
  }

protected:
  vr::IVRCompositor* native_;
};
}

#endif
