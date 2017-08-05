#ifndef NANO_ENGINE_SYSTEMS_RENDERER_RENDERER_HPP_
#define NANO_ENGINE_SYSTEMS_RENDERER_RENDERER_HPP_

#include <gl/all.hpp>

#include <nano_engine/engine.hpp>
#include <nano_engine/system.hpp>
#include <nano_engine/systems/display/display.hpp>
#include <nano_engine/systems/renderer/render_pass.hpp>

namespace ne
{
class renderer : public system
{
public:
  renderer           () : default_framebuffer_(0)
  {

  }
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
    gl::initialize            ();
    gl::set_clear_color       ({0.0F, 0.0F, 0.0F, 0.0F});
    gl::set_depth_test_enabled(true);
    for (auto& render_pass : render_passes_)
      render_pass.setup_();
  }
  void update    () override
  {
    auto size = engine_->get_system<display>()->get_windows()[0]->get_size();
    gl::set_viewport({0, 0}, {int(size[0]), int(size[1])});
    gl::clear       (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(auto& render_pass : render_passes_)
      render_pass.execute_();
  }

  gl::framebuffer          default_framebuffer_;
  std::vector<render_pass> render_passes_      ;
};
}

#endif
