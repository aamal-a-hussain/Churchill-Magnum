#ifndef INCLUDE_ENGINECORE_RENDERER_HPP_
#define INCLUDE_ENGINECORE_RENDERER_HPP_

#include "Entities/Entity.hpp"
#include "Magnum/Magnum.h"
#include "Magnum/Shaders/FlatGL.h"
#include "SpriteResource.hpp"
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
  Renderer() : m_shader{Magnum::Shaders::FlatGL2D::Configuration{}} {}

  void drawEntity(const Entity::DrawData &entity) {
    // @NOTE: Not thinking about screen resolutions now. Will deal
    // with that later
    auto &mesh = m_spriteResource.get(entity.sprite.type);
    auto pos = entity.transform.position;
    auto scale = entity.transform.scale;
    auto rot = entity.transform.rotation;

    m_shader
        .setTransformationProjectionMatrix(
            Magnum::Matrix3::translation(pos) *
            Magnum::Matrix3::scaling(scale) *
            Magnum::Matrix3::rotation(Magnum::Rad(rot)))
        .draw(mesh);
  }
};

#endif // INCLUDE_ENGINECORE_RENDERER_HPP_
