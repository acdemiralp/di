#ifndef DI_SYSTEMS_VR_MESSAGE_OVERLAY_HPP_
#define DI_SYSTEMS_VR_MESSAGE_OVERLAY_HPP_

#include <cstdint>
#include <string>
#include <vector>

#include <openvr.h>

namespace di
{
namespace message_overlay
{
static std::uint32_t create (const std::string& title, const std::string& text, const std::vector<std::string>& button_texts)
{
  return static_cast<std::uint32_t>(vr::VROverlay()->ShowMessageOverlay(
    text .c_str(), 
    title.c_str(), 
    button_texts.size() > 0 ? button_texts[0].c_str() : nullptr,
    button_texts.size() > 1 ? button_texts[1].c_str() : nullptr,
    button_texts.size() > 2 ? button_texts[2].c_str() : nullptr,
    button_texts.size() > 3 ? button_texts[3].c_str() : nullptr));
}
static void          destroy()
{
  vr::VROverlay()->CloseMessageOverlay();
}
}
}

#endif
