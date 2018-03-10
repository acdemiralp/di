#ifndef DI_SYSTEMS_VR_TRACKING_DEVICE_MODEL_HPP_
#define DI_SYSTEMS_VR_TRACKING_DEVICE_MODEL_HPP_

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <di/systems/vr/tracking_device_texture.hpp>

class ID3D11Device;
class ID3D11Texture2D;

namespace di
{
struct tracking_device_model
{
  explicit tracking_device_model  (const std::string& name) : name(name)
  {
    vr::RenderModel_t*      render_model;
    vr::EVRRenderModelError error       ;
    do
    {
      error = vr::VRRenderModels()->LoadRenderModel_Async(name.c_str(), &render_model);
    }
    while (error == vr::VRRenderModelError_Loading);
    if    (error != vr::VRRenderModelError_None   )
      throw std::runtime_error("Failed to create OpenVR render model. OpenVR Error: " + std::string(vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error)));
    
    char native_original_path[vr::k_unMaxPropertyStringSize];
    vr::VRRenderModels()->GetRenderModelOriginalPath(name.c_str(), native_original_path, vr::k_unMaxPropertyStringSize, &error);
    original_path = native_original_path;

    char native_thumbnail_url[vr::k_unMaxPropertyStringSize];
    vr::VRRenderModels()->GetRenderModelThumbnailURL(name.c_str(), native_thumbnail_url, vr::k_unMaxPropertyStringSize, &error);
    thumbnail_url = native_thumbnail_url;

    vertices           .resize(static_cast<std::size_t>(    render_model->unVertexCount  ));
    normals            .resize(static_cast<std::size_t>(    render_model->unVertexCount  ));
    texture_coordinates.resize(static_cast<std::size_t>(    render_model->unVertexCount  ));
    indices            .resize(static_cast<std::size_t>(3 * render_model->unTriangleCount));
    std::transform(render_model->rVertexData, render_model->rVertexData + vertices           .size(), vertices           .begin(), [ ] (const vr::RenderModel_Vertex_t& iteratee)
    {
      return std::array<float, 3> {iteratee.vPosition.v[0], iteratee.vPosition.v[1], iteratee.vPosition.v[2]};
    });
    std::transform(render_model->rVertexData, render_model->rVertexData + normals            .size(), normals            .begin(), [ ] (const vr::RenderModel_Vertex_t& iteratee)
    {
      return std::array<float, 3> {iteratee.vNormal.v[0], iteratee.vNormal.v[1], iteratee.vNormal.v[2]};
    });
    std::transform(render_model->rVertexData, render_model->rVertexData + texture_coordinates.size(), texture_coordinates.begin(), [ ] (const vr::RenderModel_Vertex_t& iteratee)
    {
      return std::array<float, 2> {iteratee.rfTextureCoord[0], iteratee.rfTextureCoord[1]};
    });
    std::transform(render_model->rIndexData , render_model->rIndexData  + indices            .size(), indices            .begin(), [ ] (const std::uint16_t&            iteratee)
    {
      return static_cast<unsigned>(iteratee);
    });
    
    texture_id = render_model->diffuseTextureId;

    vr::VRRenderModels()->FreeRenderModel(render_model);
  }
  tracking_device_model           (const tracking_device_model&  that) = default;
  tracking_device_model           (      tracking_device_model&& temp) = default;
  virtual ~tracking_device_model  ()                                   = default;
  tracking_device_model& operator=(const tracking_device_model&  that) = default;
  tracking_device_model& operator=(      tracking_device_model&& temp) = default;

  std::unique_ptr<tracking_device_texture>            texture      ()                         const
  {
    vr::RenderModel_TextureMap_t* native_texture;
    vr::EVRRenderModelError       error         ;
    do
    {
      error = vr::VRRenderModels()->LoadTexture_Async(texture_id, &native_texture);
    }
    while (error == vr::VRRenderModelError_Loading);
    
    auto texture  = std::make_unique<tracking_device_texture>();
    texture->size = {static_cast<std::size_t>(native_texture->unWidth), static_cast<std::size_t>(native_texture->unHeight)};
    texture->data.resize(4 * texture->size[0] * texture->size[1]);
    std::copy(native_texture->rubTextureMapData, native_texture->rubTextureMapData + texture->data.size(), texture->data.data());

    vr::VRRenderModels()->FreeTexture(native_texture);

    return texture;
  }
  ID3D11Texture2D*                                    d3d11_texture(ID3D11Device*    device ) const
  {
    ID3D11Texture2D* texture = nullptr;

    vr::EVRRenderModelError error;
    do
    {
      error = vr::VRRenderModels()->LoadTextureD3D11_Async(texture_id, device, reinterpret_cast<void**>(&texture));
    }
    while (error == vr::VRRenderModelError_Loading);

    return texture;
  }
  void                                                d3d11_texture(ID3D11Texture2D* texture) const
  {
    vr::EVRRenderModelError error;
    do
    {
      error = vr::VRRenderModels()->LoadIntoTextureD3D11_Async(texture_id, texture);
    }
    while (error == vr::VRRenderModelError_Loading);
  }
  std::vector<std::unique_ptr<tracking_device_model>> components   ()                         const
  {
    std::vector<std::unique_ptr<tracking_device_model>> components(vr::VRRenderModels()->GetComponentCount(name.c_str()));
    for(std::uint32_t i = 0; i < components.size(); ++i)
    {
      char component_name             [vr::k_unMaxPropertyStringSize];
      char component_render_model_name[vr::k_unMaxPropertyStringSize];
      vr::VRRenderModels()->GetComponentName           (name.c_str(), i, component_name,                              vr::k_unMaxPropertyStringSize);
      vr::VRRenderModels()->GetComponentRenderModelName(name.c_str(),    component_name, component_render_model_name, vr::k_unMaxPropertyStringSize);
      components[i] = std::make_unique<tracking_device_model>(component_render_model_name);
    }
    return components;
  }
  
  std::string                       name               ;
  std::string                       original_path      ;
  std::string                       thumbnail_url      ;
  std::vector<std::array<float, 3>> vertices           ;
  std::vector<std::array<float, 3>> normals            ;
  std::vector<std::array<float, 2>> texture_coordinates;
  std::vector<unsigned>             indices            ;
  std::int32_t                      texture_id         ;
};
}

#endif