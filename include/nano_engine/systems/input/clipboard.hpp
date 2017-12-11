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
  std::string text;
  if(!SDL_HasClipboardText())
    return text;
  const auto text_native = SDL_GetClipboardText();
  text = text_native;
  SDL_free(text_native);
  return text;
}
inline void        set(const std::string& text)
{
  SDL_SetClipboardText(text.c_str());
}
}
}

#endif
