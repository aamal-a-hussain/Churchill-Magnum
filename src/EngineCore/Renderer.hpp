#ifndef INCLUDE_ENGINECORE_RENDERER_HPP_
#define INCLUDE_ENGINECORE_RENDERER_HPP_

#include "Entities/Entity.hpp"
#include "Magnum/Magnum.h"
#include "Magnum/Shaders/FlatGL.h"
#include "Resources/SpriteResource.hpp"
#include "Resources/TextureLoader.hpp"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Vector.h>

class Renderer {

  Magnum::Shaders::FlatGL2D m_shader;
  SpriteResource m_spriteResource;

public:
  Renderer()
      : m_shader{Magnum::Shaders::FlatGL2D::Configuration{}.setFlags(
            Magnum::Shaders::FlatGL2D::Flag::Textured |
            Magnum::Shaders::FlatGL2D::Flag::TextureTransformation)} {}

  void drawEntity(const Entity::DrawData &entity) {
    // @NOTE: Not thinking about screen resolutions now. Will deal
    // with that later
    auto &mesh = m_spriteResource.getMesh();
    auto &spriteData = m_spriteResource.getData(entity.sprite.type);
    auto &texture = m_spriteResource.textureLoader.get(spriteData.textureId);
    Magnum::Matrix3 &textureMatrix = spriteData.textureMatrix;

    const auto pos = entity.transform.position;
    const auto scale = entity.transform.scale;
    const auto rot = entity.transform.rotation;

    m_shader
        .setTransformationProjectionMatrix(
            Magnum::Matrix3::translation(pos) *
            Magnum::Matrix3::scaling(scale) *
            Magnum::Matrix3::rotation(Magnum::Rad(rot)))
        .bindTexture(texture)
        .setTextureMatrix(textureMatrix)
        .draw(mesh);
  }
};

#endif // INCLUDE_ENGINECORE_RENDERER_HPP_
