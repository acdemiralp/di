#ifndef NANO_ENGINE_SYSTEMS_RENDERER_RENDERER_HPP_
#define NANO_ENGINE_SYSTEMS_RENDERER_RENDERER_HPP_

#include <vector>

#include <nano_engine/system.hpp>
#include <nano_engine/systems/renderer/render_pass.hpp>

namespace ne
{
class renderer : public system
{
public:
  renderer           ()                      = default;
  renderer           (const renderer&  that) = default;
  renderer           (      renderer&& temp) = default;
  virtual ~renderer  ()                      = default;
  renderer& operator=(const renderer&  that) = default;
  renderer& operator=(      renderer&& temp) = default;

  void add_render_pass(const render_pass& render_pass)
  {
    render_passes_.push_back(render_pass);
  }

protected:
  void initialize() override
  {
    for (auto& render_pass : render_passes_)
      render_pass.setup_();
  }
  void update    () override
  {
    for(auto& render_pass : render_passes_)
      render_pass.execute_();
  }

  std::vector<render_pass> render_passes_;
};
}

#endif
