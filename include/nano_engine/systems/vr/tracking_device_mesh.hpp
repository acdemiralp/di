#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_MESH_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_MESH_HPP_

#include <array>
#include <vector>

namespace ne
{
struct tracking_device_mesh
{
  std::vector<std::array<float, 3>> vertices           ;
  std::vector<std::array<float, 3>> normals            ;
  std::vector<std::array<float, 2>> texture_coordinates;
  std::vector<unsigned>             indices            ;
};
}

#endif