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
  auto display_system  = engine.add_system<ne::display> ();
  auto input_system    = engine.add_system<ne::input>   ();
  auto renderer_system = engine.add_system<ne::renderer>();
  display_system->create_window();
  engine.run();
}