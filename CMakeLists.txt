##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project               (di VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)

##################################################    Options     ##################################################
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
set(PROJECT_SOURCES
  CMakeLists.txt
  cmake/assign_source_group.cmake
  cmake/import_library.cmake
  
  include/di/systems/display/display_info.hpp
  include/di/systems/display/display_mode.hpp
  include/di/systems/display/display_system.hpp
  include/di/systems/display/hit_test_result.hpp
  include/di/systems/display/message_box.hpp
  include/di/systems/display/message_box_color_scheme.hpp
  include/di/systems/display/message_box_level.hpp
  include/di/systems/display/opengl_context_settings.hpp
  include/di/systems/display/opengl_profile.hpp
  include/di/systems/display/opengl_swap_mode.hpp
  include/di/systems/display/opengl_window.hpp
  include/di/systems/display/screensaver.hpp
  include/di/systems/display/video_driver.hpp
  include/di/systems/display/vulkan_window.hpp
  include/di/systems/display/window.hpp
  include/di/systems/display/window_flags.hpp
  include/di/systems/display/window_mode.hpp
  include/di/systems/input/clipboard.hpp
  include/di/systems/input/finger.hpp
  include/di/systems/input/game_controller.hpp
  include/di/systems/input/game_controller_axis.hpp
  include/di/systems/input/game_controller_button.hpp
  include/di/systems/input/game_controller_default_mappings.hpp
  include/di/systems/input/game_controller_info.hpp
  include/di/systems/input/gesture.hpp
  include/di/systems/input/haptic_device.hpp
  include/di/systems/input/haptic_device_info.hpp
  include/di/systems/input/haptic_effect.hpp
  include/di/systems/input/haptic_effect_description.hpp
  include/di/systems/input/input_system.hpp
  include/di/systems/input/joystick.hpp
  include/di/systems/input/joystick_hat_state.hpp
  include/di/systems/input/joystick_info.hpp
  include/di/systems/input/joystick_power_level.hpp
  include/di/systems/input/joystick_type.hpp
  include/di/systems/input/key.hpp
  include/di/systems/input/key_code.hpp
  include/di/systems/input/key_modifier.hpp
  include/di/systems/input/keyboard.hpp
  include/di/systems/input/mouse.hpp
  include/di/systems/input/multi_gesture.hpp
  include/di/systems/input/os_cursor.hpp
  include/di/systems/input/power_info.hpp
  include/di/systems/input/power_state.hpp
  include/di/systems/input/scan_code.hpp
  include/di/systems/input/touch_device.hpp
  include/di/systems/vr/camera_frame_header.hpp
  include/di/systems/vr/camera_frame_type.hpp
  include/di/systems/vr/camera_stream.hpp
  include/di/systems/vr/chaperone.hpp
  include/di/systems/vr/chaperone_calibration_state.hpp
  include/di/systems/vr/color_space.hpp
  include/di/systems/vr/dashboard_overlay.hpp
  include/di/systems/vr/direction.hpp
  include/di/systems/vr/display_redirect.hpp
  include/di/systems/vr/eye.hpp
  include/di/systems/vr/hand.hpp
  include/di/systems/vr/hmd.hpp
  include/di/systems/vr/message_overlay.hpp
  include/di/systems/vr/mirror_texture_d3d11.hpp
  include/di/systems/vr/mirror_texture_opengl.hpp
  include/di/systems/vr/overlay.hpp
  include/di/systems/vr/raycast_result.hpp
  include/di/systems/vr/statistics.hpp
  include/di/systems/vr/submit_flags.hpp
  include/di/systems/vr/texture_data_d3d12.hpp
  include/di/systems/vr/texture_data_vulkan.hpp
  include/di/systems/vr/timing_info.hpp
  include/di/systems/vr/timing_mode.hpp
  include/di/systems/vr/tracking_device.hpp
  include/di/systems/vr/tracking_device_activity.hpp
  include/di/systems/vr/tracking_device_model.hpp
  include/di/systems/vr/tracking_device_pose.hpp
  include/di/systems/vr/tracking_device_state.hpp
  include/di/systems/vr/tracking_device_texture.hpp
  include/di/systems/vr/tracking_device_type.hpp
  include/di/systems/vr/tracking_mode.hpp
  include/di/systems/vr/tracking_reference.hpp
  include/di/systems/vr/vr_controller.hpp
  include/di/systems/vr/vr_controller_axis.hpp
  include/di/systems/vr/vr_controller_button.hpp
  include/di/systems/vr/vr_controller_button_state.hpp
  include/di/systems/vr/vr_controller_state.hpp
  include/di/systems/vr/vr_screenshot.hpp
  include/di/systems/vr/vr_screenshot_type.hpp
  include/di/systems/vr/vr_system.hpp
  include/di/utility/bitset_enum.hpp
  include/di/utility/frame_timer.hpp
  include/di/utility/rectangle.hpp
  include/di/engine.hpp
  include/di/system.hpp
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
  list(APPEND PROJECT_LIBRARIES winmm.lib Imm32.lib Dwmapi.lib)
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
