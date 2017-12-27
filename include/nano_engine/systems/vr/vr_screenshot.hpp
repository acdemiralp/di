#ifndef NANO_ENGINE_SYSTEMS_VR_VR_SCREENSHOT_HPP_
#define NANO_ENGINE_SYSTEMS_VR_VR_SCREENSHOT_HPP_

#include <cstdint>
#include <string>

#include <openvr.h>

#include <nano_engine/systems/vr/vr_screenshot_type.hpp>

namespace ne
{
class vr_screenshot
{
public:
  explicit vr_screenshot  (const std::uint32_t id) : id_(id)
  {
    
  }
  vr_screenshot           (const vr_screenshot&  that) = default;
  vr_screenshot           (      vr_screenshot&& temp) = default;
  virtual ~vr_screenshot  ()                           = default;
  vr_screenshot& operator=(const vr_screenshot&  that) = default;
  vr_screenshot& operator=(      vr_screenshot&& temp) = default;

  vr_screenshot_type type            () const
  {
    vr::EVRScreenshotError error;
    return static_cast<vr_screenshot_type>(vr::VRScreenshots()->GetScreenshotPropertyType(id_, &error));
  }
  std::string        preview_filename() const
  {
    char name[vr::k_unMaxPropertyStringSize];
    vr::EVRScreenshotError error;
    vr::VRScreenshots()->GetScreenshotPropertyFilename(id_, vr::VRScreenshotPropertyFilenames_Preview, name, vr::k_unMaxPropertyStringSize, &error);
    return std::string(name);
  }
  std::string        filename        () const
  {
    char name[vr::k_unMaxPropertyStringSize];
    vr::EVRScreenshotError error;
    vr::VRScreenshots()->GetScreenshotPropertyFilename(id_, vr::VRScreenshotPropertyFilenames_VR     , name, vr::k_unMaxPropertyStringSize, &error);
    return std::string(name);
  }
  void               submit_to_steam () const
  {
    vr::VRScreenshots()->SubmitScreenshot(id_, static_cast<vr::EVRScreenshotType>(type()), preview_filename().c_str(), filename().c_str());
  }
  void               set_progress    (const float progress)
  {
    vr::VRScreenshots()->UpdateScreenshotProgress(id_, progress);
  }

protected:
  std::uint32_t id_ = 0;
};
}

#endif