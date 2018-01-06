#ifndef DI_SYSTEMS_DISPLAY_MESSAGE_BOX_HPP_
#define DI_SYSTEMS_DISPLAY_MESSAGE_BOX_HPP_

#include <cstdint>
#include <string>
#include <vector>

#include <SDL2/SDL_messagebox.h>

#include <di/systems/display/message_box_color_scheme.hpp>
#include <di/systems/display/message_box_level.hpp>

namespace di
{
namespace message_box
{
inline void        create(
  const std::string&               title        , 
  const std::string&               message      , 
  const message_box_level&         level        = message_box_level::information)
{
  SDL_ShowSimpleMessageBox(static_cast<std::uint32_t>(level), title.c_str(), message.c_str(), nullptr);
}

inline std::string create(
  const std::string&               title        , 
  const std::string&               message      , 
  const std::vector<std::string>&  buttons      , 
  const message_box_level&         level        = message_box_level::information, 
  const message_box_color_scheme&  color_scheme = message_box_color_scheme())
{
  std::vector<SDL_MessageBoxButtonData> native_buttons;
  for(int i = buttons.size() - 1; i >= 0; --i)
    native_buttons.push_back({0, i, buttons[i].c_str()});

  auto native_color_scheme = color_scheme.native();

  const SDL_MessageBoxData message_box_data
  {
    static_cast<std::uint32_t>(level), 
    nullptr                          , 
    title  .c_str()                  , 
    message.c_str()                  ,
    static_cast<int>(buttons.size()) ,
    native_buttons.data()            ,
    &native_color_scheme
  };

  auto index = 0;
  SDL_ShowMessageBox(&message_box_data, &index);
  return buttons[index];
}
}
}

#endif
