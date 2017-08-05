#ifndef NANO_ENGINE_SYSTEMS_RENDERER_RENDER_PASS_HPP_
#define NANO_ENGINE_SYSTEMS_RENDERER_RENDER_PASS_HPP_

#include <functional>

namespace ne
{
struct render_pass
{
  std::function<void()> setup_  ;
  std::function<void()> execute_;
};
}

#endif
