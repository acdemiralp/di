#ifndef NANO_ENGINE_SYSTEMS_INPUT_CLIPBOARD_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_CLIPBOARD_HPP_

#include <string>

#include <SDL2/SDL_clipboard.h>

namespace ne
{
namespace clipboard
{
inline std::string get()
{
  return std::string(SDL_GetClipboardText());
}
inline void        set(const std::string& text)
{
  SDL_SetClipboardText(text.c_str());
}
}
}

#endif
