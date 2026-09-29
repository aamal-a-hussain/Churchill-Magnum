#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Input/InputSystem.h"
#include "Resources/SpriteResource.h"

#include "Magnum/Magnum.h"
#include "Utilities/EventManager.hpp"

#include <cstddef>

class Entity {

public:
    /*
     * @brief Create an empty entity
     *
     * Creates an entity with transform initialised to
     *   position: {0.0, 0.0}
     *   scale: {1.0, 1.0}
     *   rotation: 0.0;
     *
     * Sprite is initialised to FALLBACK (i.e. will not be drawn)
     *
     */
    Entity() : transform(), sprite() {

        this->transform.position = {0.0f, 0.0f};
        this->transform.scale = {1.0f, 1.0f};
        this->transform.rotation = 0.0f;
    }

    /*
     * @brief Create an entity with a sprite
     *
     * Creates an entity with transform initialised to
     *   position: {0.0, 0.0}
     *   scale: {1.0, 1.0}
     *   rotation: 0.0;
     *
     */
    explicit Entity(const SpriteResource::SpriteType spriteType) : transform(), sprite(spriteType) {

        this->transform.position = {0.0f, 0.0f};
        this->transform.scale = {1.0f, 1.0f};
        this->transform.rotation = 0.0f;
    }

    /*
     * @brief Create an entity with a sprite and transform
     *
     */
    Entity(const Transform& transform, const SpriteResource::SpriteType spriteType) :
        transform(transform), sprite(spriteType) {}

    /**
     * @brief Set the scale of the transform
     *
     * @param scale the desired scale of the transform
     */
    void setScale(const Magnum::Vector2 scale) { transform.scale = scale; }

    /**
     * @brief Set the position of the transform.
     *
     * @param position the position of the transform.
     */
    void setPosition(const Magnum::Vector2 position) { transform.position = position; }

    /**
     * @brief Subscribe to an Input System
     *
     * @param inputSystem. Adds a callback to the input system.
     */
    void subscribeToInput(InputSystem& inputSystem) {
        game_input = inputSystem.OnMovePressed->subscribe([](const EmptyEventArgs&) {});
    }

    void update() {}

private:
    Transform transform;
    Sprite sprite;
    EventManager<EmptyEventArgs>::SubscriptionPtr game_input;
};

#endif // !ENTITY_HPP
