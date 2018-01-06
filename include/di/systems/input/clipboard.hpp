#ifndef DI_SYSTEMS_INPUT_CLIPBOARD_HPP_
#define DI_SYSTEMS_INPUT_CLIPBOARD_HPP_

#include <string>

#include <SDL2/SDL_clipboard.h>

namespace di
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
