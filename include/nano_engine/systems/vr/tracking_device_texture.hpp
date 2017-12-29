#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_TEXTURE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_DEVICE_TEXTURE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace ne
{
struct tracking_device_texture
{
  std::array <std::size_t, 2> size;
  std::vector<std::uint8_t>   data;
};
}

#endif