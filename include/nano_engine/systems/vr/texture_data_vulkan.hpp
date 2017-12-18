#ifndef NANO_ENGINE_SYSTEMS_VR_TEXTURE_DATA_VULKAN_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TEXTURE_DATA_VULKAN_HPP_

#include <cstdint>

#include <openvr.h>

typedef std::uint64_t              VkImage         ;
typedef struct VkDevice_T*         VkDevice        ;
typedef struct VkInstance_T*       VkInstance      ;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkQueue_T*          VkQueue         ;

namespace ne
{
struct texture_data_vulkan
{
  VkImage          image                              ;
  VkDevice         device                             ;
  VkPhysicalDevice physical_device                    ;
  VkInstance       instance                           ;
  VkQueue          queue                              ;
  std::uint32_t    queue_family_index                 ;
  std::uint32_t    width, height, format, sample_count;
};
}

#endif