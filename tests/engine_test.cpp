#include "catch.hpp"

#include <nano_engine/systems/display.hpp>
#include <nano_engine/systems/input.hpp>
#include <nano_engine/systems/renderer.hpp>
#include <nano_engine/engine.hpp>

TEST_CASE("Engine is tested.", "[engine]") {
  ne::engine engine;
  auto display_system    = engine.add_system<ne::display> ();
  auto input_system      = engine.add_system<ne::input>   ();
  auto renderer_system   = engine.add_system<ne::renderer>();
  
  auto display_system_2  = engine.get_system<ne::display> ();
  auto input_system_2    = engine.get_system<ne::input>   ();
  auto renderer_system_2 = engine.get_system<ne::renderer>();

  REQUIRE(display_system_2  == display_system );
  REQUIRE(input_system_2    == input_system   );
  REQUIRE(renderer_system_2 == renderer_system);

  engine.remove_system<ne::renderer>();
  
  auto display_system_3  = engine.get_system<ne::display> ();
  auto input_system_3    = engine.get_system<ne::input>   ();
  auto renderer_system_3 = engine.get_system<ne::renderer>();
  
  REQUIRE(display_system_3  == display_system);
  REQUIRE(input_system_3    == input_system  );
  REQUIRE(renderer_system_3 == nullptr       );

  auto renderer_system_4 = engine.add_system<ne::renderer>();

  engine.run();
}