#ifndef DI_SYSTEMS_VR_CHAPERONE_HPP_
#define DI_SYSTEMS_VR_CHAPERONE_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

#include <openvr.h>

#include <di/systems/vr/chaperone_calibration_state.hpp>

namespace di
{
class chaperone
{
public:
  // IVR Chaperone
  chaperone_calibration_state                      calibration_state                              ()                                                              const
  {
    return static_cast<chaperone_calibration_state>(vr::VRChaperone()->GetCalibrationState());
  }
  std::array<std::array<float, 2>, 4>              play_area                                      ()                                                              const
  {
    vr::HmdQuad_t quad;
    vr::VRChaperone()->GetPlayAreaRect(&quad);
    return std::array<std::array<float, 2>, 4> 
    {
      std::array<float, 2>{quad.vCorners[0].v[0], quad.vCorners[0].v[2]},
      std::array<float, 2>{quad.vCorners[1].v[0], quad.vCorners[1].v[2]},
      std::array<float, 2>{quad.vCorners[2].v[0], quad.vCorners[2].v[2]},
      std::array<float, 2>{quad.vCorners[3].v[0], quad.vCorners[3].v[2]}
    };
  }
  std::array<float, 2>                             play_area_size                                 ()                                                              const
  {
    std::array<float, 2> size;
    vr::VRChaperone()->GetPlayAreaSize(&size[0], &size[1]);
    return size;
  }
                                                                                                                                                                  
  std::array<float, 4>                             color                                          ()                                                              const
  {
    vr::HmdColor_t color, output_camera_color;
    vr::VRChaperone()->GetBoundsColor(&color, 1, 0.0F, &output_camera_color);
    return std::array<float, 4>{color.r, color.g, color.b, color.a};
  }
  void                                             set_color                                      (const std::array<float, 4>& color)                            
  {
    vr::VRChaperone()->SetSceneColor(vr::HmdColor_t{color[0], color[1], color[2], color[3]});
  }                    
                                                                                                                                                                  
  bool                                             visible                                        ()                                                              const
  {
    return vr::VRChaperone()->AreBoundsVisible();
  }
  void                                             set_visible                                    (const bool visible)
  {
    vr::VRChaperone()->ForceBoundsVisible(visible);
  }                                         
                                                                                                                                                                  
  void                                             reload                                         ()                                                              const
  {
    vr::VRChaperone()->ReloadInfo();
  }
                                                                                                                                                                  
  // IVR Chaperone Setup                                                                                                                                          
  std::array<std::array<float, 2>, 4>              working_play_area                              ()                                                              const
  {
    vr::HmdQuad_t quad;
    vr::VRChaperoneSetup()->GetWorkingPlayAreaRect(&quad);
    return std::array<std::array<float, 2>, 4> 
    {
      std::array<float, 2>{quad.vCorners[0].v[0], quad.vCorners[0].v[2]},
      std::array<float, 2>{quad.vCorners[1].v[0], quad.vCorners[1].v[2]},
      std::array<float, 2>{quad.vCorners[2].v[0], quad.vCorners[2].v[2]},
      std::array<float, 2>{quad.vCorners[3].v[0], quad.vCorners[3].v[2]}
    };
  }
  std::array<float, 2>                             working_play_area_size                         ()                                                              const
  {
    std::array<float, 2> size;
    vr::VRChaperoneSetup()->GetWorkingPlayAreaSize(&size[0], &size[1]);
    return size;
  }
  void                                             set_working_play_area_size                     (const std::array<float, 2>& size)                            
  {
    vr::VRChaperoneSetup()->SetWorkingPlayAreaSize(size[0], size[1]);
  }                     
                                                                                                                                                                  
  std::array<float, 12>                            seated_zero_pose_to_absolute_pose              ()                                                              const
  {
    std::array<float, 12> matrix       ;
    vr::HmdMatrix34_t     native_matrix;
    vr::VRChaperoneSetup()->GetLiveSeatedZeroPoseToRawTrackingPose(&native_matrix);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  std::array<float, 12>                            working_seated_zero_pose_to_absolute_pose      ()                                                              const
  {
    std::array<float, 12> matrix       ;
    vr::HmdMatrix34_t     native_matrix;
    vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(&native_matrix);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  void                                             set_working_seated_zero_pose_to_absolute_pose  (const std::array<float, 12>& matrix)                         
  {
    vr::HmdMatrix34_t native_matrix;
    std::copy(matrix.begin(), matrix.end(), &native_matrix.m[0][0]);
    vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(&native_matrix);
  }                  
                                                                                                                                                                  
  std::array<float, 12>                            working_standing_zero_pose_to_absolute_pose    ()                                                              const
  {
    std::array<float, 12> matrix       ;
    vr::HmdMatrix34_t     native_matrix;
    vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&native_matrix);
    std::copy(&native_matrix.m[0][0], &native_matrix.m[0][0] + 12, matrix.begin());
    return matrix;
  }
  void                                             set_working_standing_zero_pose_to_absolute_pose(const std::array<float, 12>& matrix)                         
  {
    vr::HmdMatrix34_t native_matrix;
    std::copy(matrix.begin(), matrix.end(), &native_matrix.m[0][0]);
    vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&native_matrix);
  }                  
                                                                                                                                                               
  std::vector<std::array<std::array<float, 2>, 4>> collision_bounds                               ()                                                              const
  {
    std::uint32_t size;
    vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(nullptr, &size);

    std::vector<vr::HmdQuad_t>                       native_quads(size);
    std::vector<std::array<std::array<float, 2>, 4>> quads       (size);
    vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(native_quads.data(), &size);
    std::transform(native_quads.begin(), native_quads.end(), quads.begin(), [ ] (const vr::HmdQuad_t& iteratee)
    { 
      return std::array<std::array<float, 2>, 4> 
      {
        std::array<float, 2>{iteratee.vCorners[0].v[0], iteratee.vCorners[0].v[2]},
        std::array<float, 2>{iteratee.vCorners[1].v[0], iteratee.vCorners[1].v[2]},
        std::array<float, 2>{iteratee.vCorners[2].v[0], iteratee.vCorners[2].v[2]},
        std::array<float, 2>{iteratee.vCorners[3].v[0], iteratee.vCorners[3].v[2]}
      };
    });
    return quads;
  }
  std::vector<std::array<std::array<float, 2>, 4>> working_collision_bounds                       ()                                                              const
  {
    std::uint32_t size;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(nullptr, &size);

    std::vector<vr::HmdQuad_t>                       native_quads(size);
    std::vector<std::array<std::array<float, 2>, 4>> quads       (size);
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(native_quads.data(), &size);
    std::transform(native_quads.begin(), native_quads.end(), quads.begin(), [ ] (const vr::HmdQuad_t& iteratee)
    { 
      return std::array<std::array<float, 2>, 4> 
      {
        std::array<float, 2>{iteratee.vCorners[0].v[0], iteratee.vCorners[0].v[2]},
        std::array<float, 2>{iteratee.vCorners[1].v[0], iteratee.vCorners[1].v[2]},
        std::array<float, 2>{iteratee.vCorners[2].v[0], iteratee.vCorners[2].v[2]},
        std::array<float, 2>{iteratee.vCorners[3].v[0], iteratee.vCorners[3].v[2]}
      };
    });
    return quads;
  }
  void                                             set_working_collision_bounds                   (const std::vector<std::array<std::array<float, 2>, 4>>& quads)
  {
    std::vector<vr::HmdQuad_t> native_quads(quads.size());
    std::transform(quads.begin(), quads.end(), native_quads.begin(), [ ] (const std::array<std::array<float, 2>, 4>& iteratee)
    {
      return vr::HmdQuad_t
      {
        vr::HmdVector3_t{iteratee[0][0], 0.0F, iteratee[0][2]},
        vr::HmdVector3_t{iteratee[1][0], 0.0F, iteratee[1][2]},
        vr::HmdVector3_t{iteratee[2][0], 0.0F, iteratee[2][2]},
        vr::HmdVector3_t{iteratee[3][0], 0.0F, iteratee[3][2]}
      };
    });
    vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(native_quads.data(), static_cast<std::uint32_t>(native_quads.size()));
  }
                         
  std::vector<std::uint8_t>                        collision_bounds_tags                          ()                                                              const
  {
    std::uint32_t size;
    vr::VRChaperoneSetup()->GetLiveCollisionBoundsTagsInfo(nullptr, &size);

    std::vector<std::uint8_t> tags(size);
    vr::VRChaperoneSetup()->GetLiveCollisionBoundsTagsInfo(tags.data(), &size);
    return tags;
  }
  void                                             set_working_collision_bounds_tags              (const std::vector<std::uint8_t>& tags)
  {
    vr::VRChaperoneSetup()->SetWorkingCollisionBoundsTagsInfo(const_cast<std::uint8_t*>(tags.data()), static_cast<std::uint32_t>(tags.size()));
  }
                                               
  std::vector<std::array<std::array<float, 2>, 4>> physical_bounds                                ()                                                              const
  {
    std::uint32_t size;
    vr::VRChaperoneSetup()->GetLivePhysicalBoundsInfo(nullptr, &size);

    std::vector<vr::HmdQuad_t>                       native_quads(size);
    std::vector<std::array<std::array<float, 2>, 4>> quads       (size);
    vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(native_quads.data(), &size);
    std::transform(native_quads.begin(), native_quads.end(), quads.begin(), [ ] (const vr::HmdQuad_t& iteratee)
    { 
      return std::array<std::array<float, 2>, 4> 
      {
        std::array<float, 2>{iteratee.vCorners[0].v[0], iteratee.vCorners[0].v[2]},
        std::array<float, 2>{iteratee.vCorners[1].v[0], iteratee.vCorners[1].v[2]},
        std::array<float, 2>{iteratee.vCorners[2].v[0], iteratee.vCorners[2].v[2]},
        std::array<float, 2>{iteratee.vCorners[3].v[0], iteratee.vCorners[3].v[2]}
      };
    });
    return quads;
  }                                                
  void                                             set_working_physical_bounds                    (const std::vector<std::array<std::array<float, 2>, 4>>& quads)
  {        
    std::vector<vr::HmdQuad_t> native_quads(quads.size());
    std::transform(quads.begin(), quads.end(), native_quads.begin(), [ ] (const std::array<std::array<float, 2>, 4>& iteratee)
    {
      return vr::HmdQuad_t
      {
        vr::HmdVector3_t{iteratee[0][0], 0.0F, iteratee[0][2]},
        vr::HmdVector3_t{iteratee[1][0], 0.0F, iteratee[1][2]},
        vr::HmdVector3_t{iteratee[2][0], 0.0F, iteratee[2][2]},
        vr::HmdVector3_t{iteratee[3][0], 0.0F, iteratee[3][2]}
      };
    });
    vr::VRChaperoneSetup()->SetWorkingPhysicalBoundsInfo(native_quads.data(), static_cast<std::uint32_t>(native_quads.size()));                                      
  }                                                
                                                   
  void                                             commit                                         (const bool temporary = false)                                  const
  {
    vr::VRChaperoneSetup()->CommitWorkingCopy(temporary ? vr::EChaperoneConfigFile_Temp : vr::EChaperoneConfigFile_Live);
  }
  void                                             revert                                         ()                                                              const
  {
    vr::VRChaperoneSetup()->RevertWorkingCopy();
  }                                                            
                                                                                                                                                                   
  void                                             export_to_buffer                               ()                                                              const
  {
    std::uint32_t size;
    vr::VRChaperoneSetup()->ExportLiveToBuffer(nullptr   , &size);
    std::string   buffer(size, ' ');
    vr::VRChaperoneSetup()->ExportLiveToBuffer(&buffer[0], &size);
  }                                                            
  void                                             import_from_buffer                             (const std::string& buffer)                                     const
  {
    vr::VRChaperoneSetup()->ImportFromBufferToWorking(buffer.data(), vr::EChaperoneImport_BoundsOnly);
  }
                                                                                                                                                                   
  void                                             reload                                         (const bool temporary)                                          const
  {
    vr::VRChaperoneSetup()->ReloadFromDisk(temporary ? vr::EChaperoneConfigFile_Temp : vr::EChaperoneConfigFile_Live);
  }
};
}

#endif
