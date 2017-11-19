#ifndef NANO_ENGINE_SYSTEMS_INPUT_CLIPBOARD_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_CLIPBOARD_HPP_

#include <string>

#include <SDL2/SDL_clipboard.h>

namespace ne
{
inline std::string clipboard_text    ()
{
  return std::string(SDL_GetClipboardText());
}
inline void        set_clipboard_text(const std::string& text)
{
  SDL_SetClipboardText(text.c_str());
}
}

#endif
