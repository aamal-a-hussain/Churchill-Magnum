#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "EngineCore/Input/InputSystem.h"
#include "EngineCore/Resources/SpriteResource.hpp"

#include "Corrade/Containers/Optional.h"
#include "Corrade/Containers/StaticArray.h"
#include "EngineCore/Animation/SpriteAnimationController.hpp"
#include "Magnum/Magnum.h"
#include "Utilities/EventManager.hpp"

#include <cstddef>

class Entity {

public:
  static constexpr std::size_t N_ANIM_FRAMES = 8;
  Entity() : m_transform(), m_sprite() {

    // @NOTE:
    // Making the assumption that the entity transform
    // is the same as the sprite transform. I.e. the
    // sprite is always centred at the entity position
    m_transform.position = {0.0f, 0.0f};
    m_transform.scale = {1.0f, 1.0f};
    m_transform.rotation = 0.0f;
  }
  Entity(const SpriteResource::SpriteType spriteType)
      : m_transform(), m_sprite(spriteType) {

    m_transform.position = {0.0f, 0.0f};
    m_transform.scale = {1.0f, 1.0f};
    m_transform.rotation = 0.0f;
  }

  Entity(const Transform &transform,
         const SpriteResource::SpriteType spriteType)
      : m_transform(transform), m_sprite(spriteType) {}

  struct DrawData {
    Sprite sprite;
    const Transform &transform;
  };

  const DrawData getDrawData() const { return DrawData{m_sprite, m_transform}; }

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

  void subscribeToInput(InputSystem &inputSystem) {
    m_game_input = inputSystem.OnMovePressed->subscribe(
        [this](const EmptyEventArgs &args) {
          this->m_transform.position += Magnum::Vector2{0.01f, 0.0f};
        });
  }

  void setAnimation(
      Corrade::Containers::StaticArray<N_ANIM_FRAMES, Sprite> animation) {
    CORRADE_ASSERT(!m_animationController,
                   "Animation Controller has already been set and we have not "
                   "yet handled overwriting animations", );
    m_animationController =
        Animation::SpriteAnimationController<N_ANIM_FRAMES>{animation, 30};
  }

  void update() {
    if (m_animationController) {
      (*m_animationController).tick();
      m_sprite = (*m_animationController).getCurrentFrame();
    }
  }

private:
  Transform m_transform;
  Sprite m_sprite;
  EventManager<EmptyEventArgs>::SubscriptionPtr m_game_input;
  Corrade::Containers::Optional<
      Animation::SpriteAnimationController<N_ANIM_FRAMES>>
      m_animationController;
};

#endif // !ENTITY_HPP
