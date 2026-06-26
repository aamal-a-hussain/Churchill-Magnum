#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../Components/Sprite.hpp"
#include "../Components/Transform.hpp"
#include "../EngineCore/SpriteResource.hpp"
#include "Magnum/Magnum.h"
#include <format>

class Entity {

public:
  Entity() : m_transform(), m_sprite(), m_draw_data(m_sprite, m_transform) {

    // @NOTE:
    // Making the assumption that the entity transform
    // is the same as the sprite transform. I.e. the
    // sprite is always centred at the entity position
    m_transform.position = {0.0f, 0.0f};
    m_transform.scale = {1.0f, 1.0f};
    m_transform.rotation = 0.0f;
  }

  Entity(const Transform &transform,
         const SpriteResource::SpriteType spriteType)
      : m_transform(transform), m_sprite(spriteType),
        m_draw_data(m_sprite, m_transform) {}

  struct DrawData {
    Sprite sprite;
    Transform &transform;
  };

  const DrawData &getDrawData() const { return m_draw_data; }

  void setScale(const Magnum::Vector2 scale) {
    CORRADE_ASSERT(
        scale > Magnum::Vector2{} && scale <= Magnum::Vector2{1.0f},
        std::format(
            "Setting an Entity scale with scale [{}, {}] not in (0, 1].",
            scale.x(), scale.y())
            .c_str(), );
    m_transform.scale = scale;
  }
  void setPosition(const Magnum::Vector2 position) {
    CORRADE_ASSERT(position >= Magnum::Vector2{-1.0f} &&
                       position <= Magnum::Vector2{1.0f},
                   std::format("position [{}, {}] is out of bounds",
                               position.x(), position.y())
                       .c_str(), );
    m_transform.position = position;
  }

private:
  Transform m_transform;
  const Sprite m_sprite;

  // Ensure DrawData is last!
  DrawData m_draw_data;
};

#endif // !ENTITY_HPP
