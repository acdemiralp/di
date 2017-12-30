#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_MODEL_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_MODEL_HPP_

#include <array>
#include <vector>

#include <nano_engine/systems/vr/tracking_device_texture.hpp>

namespace ne
{
struct tracking_device_model
{
  std::string                       name               ;
  std::string                       original_path      ;
  std::string                       thumbnail_url      ;
  std::vector<std::array<float, 3>> vertices           ;
  std::vector<std::array<float, 3>> normals            ;
  std::vector<std::array<float, 2>> texture_coordinates;
  std::vector<unsigned>             indices            ;
  tracking_device_texture           diffuse_texture    ;
};
}

#endif