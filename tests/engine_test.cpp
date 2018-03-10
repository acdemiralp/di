#include "catch.hpp"

#include <iostream>

#include <di/systems/display/display_system.hpp>
#include <di/systems/input/input_system.hpp>
#include <di/systems/vr/vr_system.hpp>
#include <di/engine.hpp>

extern "C"
{
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}

TEST_CASE("Engine is tested.", "[engine]") {
  di::engine engine;
  auto display_system  = engine.add_system<di::display_system>();
  auto input_system    = engine.add_system<di::input_system>  ();
  if(di::vr_system::available()) 
    engine.add_system<di::vr_system>(di::tracking_mode::seated);
  auto opengl_window = display_system->create_opengl_window("Test", std::array<std::size_t, 2>{32, 32}, std::array<std::size_t, 2>{640, 480});
  input_system ->on_key_press.connect([ ] (di::key key) { std::cout << key.name(); });
  engine.run();
}