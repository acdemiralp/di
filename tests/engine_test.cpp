#include "catch.hpp"

#include <nano_engine/systems/display/display.hpp>
#include <nano_engine/systems/input/input.hpp>
#include <nano_engine/systems/renderer/renderer.hpp>
#include <nano_engine/engine.hpp>

#ifdef _WIN32
extern "C"
{
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}
#endif

TEST_CASE("Engine is tested.", "[engine]") {
  ne::engine engine;
  auto input_system    = engine.add_system<ne::input>   ();
  auto renderer_system = engine.add_system<ne::renderer>();
  auto display_system  = engine.add_system<ne::display> ();
  display_system ->create_window  ();

  std::unique_ptr<gl::program>      shader_program;
  std::unique_ptr<gl::vertex_array> vertex_array  ;
  renderer_system->add_render_pass(ne::render_pass
  {
    [&shader_program, &vertex_array] ()
    {
      shader_program = std::make_unique<gl::program>     ();
      vertex_array   = std::make_unique<gl::vertex_array>();

      gl::shader vertex_shader  (GL_VERTEX_SHADER  );
      gl::shader fragment_shader(GL_FRAGMENT_SHADER);
      vertex_shader  .set_source("#version 420 \n layout(location = 0) in vec3 position; out vec3 color; void main() { color = position; gl_Position = vec4(position, 1.0); }");
      fragment_shader.set_source("#version 420 \n in vec3 color; out vec4 frag_color; void main() { frag_color = vec4(color.x + 0.5, 0.5, color.y + 0.5, 1.0); }");
      vertex_shader  .compile();
      fragment_shader.compile();

      shader_program->attach_shader(vertex_shader  );
      shader_program->attach_shader(fragment_shader);
      shader_program->link();

      float points[] = { -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
      gl::buffer vertex_buffer;
      vertex_buffer.set_data_immutable(16 * sizeof(float), points);

      vertex_array->set_attribute_enabled(0, true);
      vertex_array->set_vertex_buffer    (0, vertex_buffer, 0, 3 * sizeof(float));
    },
    [&shader_program, &vertex_array] ()
    {
      vertex_array  ->bind  ();
      shader_program->use   ();

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

      shader_program->unuse ();
      vertex_array  ->unbind();
    }
  });

  engine.run();
}