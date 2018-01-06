#ifndef DI_SYSTEMS_VR_TIMING_INFO_HPP_
#define DI_SYSTEMS_VR_TIMING_INFO_HPP_

#include <chrono>
#include <cstddef>

namespace di
{
struct timing_info
{
  std::size_t                              frame_index            ;
  std::size_t                              present_count          ;
  std::size_t                              mispresent_count       ;
  std::size_t                              drop_count             ;
                                                                      
  std::chrono::duration<double>            system_time            ;
                                                            
  std::chrono::duration<float, std::milli> pre_submit             ;
  std::chrono::duration<float, std::milli> post_submit            ;
  std::chrono::duration<float, std::milli> total_gpu              ;
  std::chrono::duration<float, std::milli> compositor_gpu         ;
  std::chrono::duration<float, std::milli> compositor_cpu         ;
  std::chrono::duration<float, std::milli> compositor_cpu_idle    ;
                                                            
  std::chrono::duration<float, std::milli> wait_get_poses         ;
  std::chrono::duration<float, std::milli> present                ;
  std::chrono::duration<float, std::milli> wait_for_present       ;
  std::chrono::duration<float, std::milli> submit_frame           ;
                                                            
  std::chrono::duration<float, std::milli> wait_get_poses_called  ;
  std::chrono::duration<float, std::milli> new_poses_ready        ;
  std::chrono::duration<float, std::milli> new_frame_ready        ;
  std::chrono::duration<float, std::milli> compositor_update_start;
  std::chrono::duration<float, std::milli> compositor_update_end  ;
  std::chrono::duration<float, std::milli> compositor_render_start;
};
}

#endif
