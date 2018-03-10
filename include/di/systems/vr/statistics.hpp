#ifndef DI_SYSTEMS_VR_STATISTICS_HPP_
#define DI_SYSTEMS_VR_STATISTICS_HPP_

#include <cstddef>

namespace di
{
struct statistics
{
  std::size_t process_id                    ;

  std::size_t present_count                 ;
  std::size_t drop_count                    ;
  std::size_t reprojected_drop_count        ;

  std::size_t startup_present_count         ;
  std::size_t startup_drop_count            ;
  std::size_t startup_reprojected_drop_count;

  std::size_t loading_count                 ;
  std::size_t loading_present_count         ;
  std::size_t loading_drop_count            ;
  std::size_t loading_reprojected_drop_count;

  std::size_t timeout_count                 ;
  std::size_t timeout_present_count         ;
  std::size_t timeout_drop_count            ;
  std::size_t timeout_reprojected_drop_count;
};
}

#endif
