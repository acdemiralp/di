#ifndef DI_SYSTEMS_VR_TRACKING_DEVICE_TEXTURE_HPP_
#define DI_SYSTEMS_VR_TRACKING_DEVICE_TEXTURE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace di
{
struct tracking_device_texture
{
  std::array <std::size_t, 2> size;
  std::vector<std::uint8_t>   data;
};
}

#endif