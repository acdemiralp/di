##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project               (nano_engine VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)

##################################################    Options     ##################################################
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
set(PROJECT_SOURCES
  CMakeLists.txt
  cmake/assign_source_group.cmake
  cmake/import_library.cmake
  
  include/nano_engine/systems/display/display_info.hpp
  include/nano_engine/systems/display/display_mode.hpp
  include/nano_engine/systems/display/display_system.hpp
  include/nano_engine/systems/display/hit_test_result.hpp
  include/nano_engine/systems/display/message_box.hpp
  include/nano_engine/systems/display/message_box_color_scheme.hpp
  include/nano_engine/systems/display/message_box_level.hpp
  include/nano_engine/systems/display/opengl_context_settings.hpp
  include/nano_engine/systems/display/opengl_profile.hpp
  include/nano_engine/systems/display/opengl_swap_mode.hpp
  include/nano_engine/systems/display/opengl_window.hpp
  include/nano_engine/systems/display/screensaver.hpp
  include/nano_engine/systems/display/video_driver.hpp
  include/nano_engine/systems/display/vulkan_window.hpp
  include/nano_engine/systems/display/window.hpp
  include/nano_engine/systems/display/window_flags.hpp
  include/nano_engine/systems/display/window_mode.hpp
  include/nano_engine/systems/input/clipboard.hpp
  include/nano_engine/systems/input/finger.hpp
  include/nano_engine/systems/input/game_controller.hpp
  include/nano_engine/systems/input/game_controller_axis.hpp
  include/nano_engine/systems/input/game_controller_button.hpp
  include/nano_engine/systems/input/game_controller_default_mappings.hpp
  include/nano_engine/systems/input/game_controller_info.hpp
  include/nano_engine/systems/input/gesture.hpp
  include/nano_engine/systems/input/haptic_device.hpp
  include/nano_engine/systems/input/haptic_device_info.hpp
  include/nano_engine/systems/input/haptic_effect.hpp
  include/nano_engine/systems/input/haptic_effect_description.hpp
  include/nano_engine/systems/input/input_system.hpp
  include/nano_engine/systems/input/joystick.hpp
  include/nano_engine/systems/input/joystick_hat_state.hpp
  include/nano_engine/systems/input/joystick_info.hpp
  include/nano_engine/systems/input/joystick_power_level.hpp
  include/nano_engine/systems/input/joystick_type.hpp
  include/nano_engine/systems/input/key.hpp
  include/nano_engine/systems/input/key_code.hpp
  include/nano_engine/systems/input/key_modifier.hpp
  include/nano_engine/systems/input/keyboard.hpp
  include/nano_engine/systems/input/mouse.hpp
  include/nano_engine/systems/input/multi_gesture.hpp
  include/nano_engine/systems/input/os_cursor.hpp
  include/nano_engine/systems/input/power_info.hpp
  include/nano_engine/systems/input/power_state.hpp
  include/nano_engine/systems/input/scan_code.hpp
  include/nano_engine/systems/input/touch_device.hpp
  include/nano_engine/systems/renderer/render_pass.hpp
  include/nano_engine/systems/renderer/renderer.hpp
  include/nano_engine/systems/vr/camera_frame_header.hpp
  include/nano_engine/systems/vr/camera_frame_type.hpp
  include/nano_engine/systems/vr/camera_stream.hpp
  include/nano_engine/systems/vr/chaperone.hpp
  include/nano_engine/systems/vr/chaperone_calibration_state.hpp
  include/nano_engine/systems/vr/color_space.hpp
  include/nano_engine/systems/vr/display_redirect.hpp
  include/nano_engine/systems/vr/eye.hpp
  include/nano_engine/systems/vr/hand.hpp
  include/nano_engine/systems/vr/hmd.hpp
  include/nano_engine/systems/vr/mirror_texture_d3d11.hpp
  include/nano_engine/systems/vr/mirror_texture_opengl.hpp
  include/nano_engine/systems/vr/statistics.hpp
  include/nano_engine/systems/vr/submit_flags.hpp
  include/nano_engine/systems/vr/texture_data_d3d12.hpp
  include/nano_engine/systems/vr/texture_data_vulkan.hpp
  include/nano_engine/systems/vr/timing_info.hpp
  include/nano_engine/systems/vr/timing_mode.hpp
  include/nano_engine/systems/vr/tracking_device.hpp
  include/nano_engine/systems/vr/tracking_device_activity.hpp
  include/nano_engine/systems/vr/tracking_device_model.hpp
  include/nano_engine/systems/vr/tracking_device_pose.hpp
  include/nano_engine/systems/vr/tracking_device_state.hpp
  include/nano_engine/systems/vr/tracking_device_texture.hpp
  include/nano_engine/systems/vr/tracking_device_type.hpp
  include/nano_engine/systems/vr/tracking_mode.hpp
  include/nano_engine/systems/vr/tracking_reference.hpp
  include/nano_engine/systems/vr/vr_controller.hpp
  include/nano_engine/systems/vr/vr_controller_axis.hpp
  include/nano_engine/systems/vr/vr_controller_button.hpp
  include/nano_engine/systems/vr/vr_controller_button_state.hpp
  include/nano_engine/systems/vr/vr_controller_state.hpp
  include/nano_engine/systems/vr/vr_screenshot.hpp
  include/nano_engine/systems/vr/vr_screenshot_type.hpp
  include/nano_engine/systems/vr/vr_system.hpp
  include/nano_engine/utility/bitset_enum.hpp
  include/nano_engine/utility/frame_timer.hpp
  include/nano_engine/utility/rectangle.hpp
  include/nano_engine/engine.hpp
  include/nano_engine/system.hpp
)
include(assign_source_group)
assign_source_group(${PROJECT_SOURCES})

##################################################  Dependencies  ##################################################
include(import_library)

find_package  (Boost REQUIRED)
import_library(Boost_INCLUDE_DIRS)

find_package  (OpenVR REQUIRED)
import_library(OPENVR_INCLUDE_DIRS OPENVR_LIBRARY)

find_package  (SDL2 REQUIRED)
import_library(SDL2_INCLUDE_DIR SDL2_LIBRARY)
if   (WIN32)
  list(APPEND PROJECT_LIBRARIES winmm.lib Imm32.lib)
endif()

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} INTERFACE ${PROJECT_LIBRARIES})

# Hack for header-only project to appear in the IDEs.
add_library(${PROJECT_NAME}_ STATIC ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}_ 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME}_ PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME}_ PUBLIC ${PROJECT_LIBRARIES})
set_target_properties     (${PROJECT_NAME}_ PROPERTIES LINKER_LANGUAGE CXX)

##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing()

  set(PROJECT_TEST_SOURCES
    tests/engine_test.cpp
  )

  foreach(_SOURCE ${PROJECT_TEST_SOURCES})
    get_filename_component(_NAME ${_SOURCE} NAME_WE)
    set                   (_SOURCES tests/catch.hpp tests/main.cpp ${_SOURCE})
    add_executable        (${_NAME} ${_SOURCES})
    target_link_libraries (${_NAME} ${PROJECT_NAME})
    add_test              (${_NAME} ${_NAME})
    set_property          (TARGET ${_NAME} PROPERTY FOLDER "Tests")
    source_group          ("source" FILES ${_SOURCES})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}-config")
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  "${PROJECT_NAME}-config" DESTINATION "cmake")
export (TARGETS "${PROJECT_NAME}"        FILE        "${PROJECT_NAME}-config.cmake")
