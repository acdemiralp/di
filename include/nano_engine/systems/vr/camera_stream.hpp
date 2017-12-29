#ifndef NANO_ENGINE_SYSTEMS_VR_CAMERA_STREAM_HPP_
#define NANO_ENGINE_SYSTEMS_VR_CAMERA_STREAM_HPP_

#include <array>
#include <cstdint>

#include <openvr.h>

#include <nano_engine/systems/vr/camera_frame_type.hpp>
#include <nano_engine/utility/rectangle.hpp>

namespace ne
{
class camera_stream
{
public:
  explicit camera_stream  (const std::uint64_t& id, const camera_frame_type type = camera_frame_type::distorted) : id_(id), type_(type)
  {
    
  }
  camera_stream           (const camera_stream&  that) = default;
  camera_stream           (      camera_stream&& temp) = default;
  virtual ~camera_stream  ()
  {
    vr::VRTrackedCamera()->ReleaseVideoStreamingService(id_);
  }
  camera_stream& operator=(const camera_stream&  that) = default;
  camera_stream& operator=(      camera_stream&& temp) = default;

  std::array<std::uint32_t, 2> size  () const
  {
    vr::VRTextureBounds_t        bounds;
    std::array<std::uint32_t, 2> size  ;
    vr::VRTrackedCamera()->GetVideoStreamTextureSize(id_, static_cast<vr::EVRTrackedCameraFrameType>(type_), &bounds, &size[0], &size[1]);
    return size;
  }
  rectangle<float>             bounds() const
  {
    vr::VRTextureBounds_t        bounds;
    std::array<std::uint32_t, 2> size  ;
    vr::VRTrackedCamera()->GetVideoStreamTextureSize(id_, static_cast<vr::EVRTrackedCameraFrameType>(type_), &bounds, &size[0], &size[1]);
    return rectangle<float> {bounds.uMin, bounds.vMin, bounds.uMax, bounds.vMax};
  }
  
  // TODO START
  void header                ()
  {
    vr::CameraVideoStreamFrameHeader_t header;
    vr::VRTrackedCamera()->GetVideoStreamFrameBuffer (id_, static_cast<vr::EVRTrackedCameraFrameType>(type_), nullptr, 0, &header, sizeof vr::CameraVideoStreamFrameHeader_t);
  }
  void buffer                ()
  {
    void*         framebuffer     ;
    std::uint32_t framebuffer_size;
    vr::VRTrackedCamera()->GetVideoStreamFrameBuffer (id_, static_cast<vr::EVRTrackedCameraFrameType>(type_), framebuffer, framebuffer_size, nullptr, 0);
  }
  void d3d11_texture         ()
  {
    //vr::VRTrackedCamera()->GetVideoStreamTextureD3D11(id_, static_cast<vr::EVRTrackedCameraFrameType>(type_), )
  }
  void opengl_texture        ()
  {
    //vr::VRTrackedCamera()->GetVideoStreamTextureGL   (id_, static_cast<vr::EVRTrackedCameraFrameType>(type_), )
  }
  void release_opengl_texture()
  {
    //vr::VRTrackedCamera()->ReleaseVideoStreamTextureGL(id_, )
  }
  // TODO END

protected:
  std::uint64_t     id_  ;
  camera_frame_type type_;
};
}

#endif