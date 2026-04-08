//
// Created by aamalh on 4/2/26.
//

#ifndef CHURCHILLMAGNUM_ENTITY_H
#define CHURCHILLMAGNUM_ENTITY_H

#include "../Components/GameComponents.hpp"
#include "Magnum/Math/Matrix3.h"
#include "Magnum/Vk/Shader.h"

enum EntityType {
    Player,
    Enemy,
    Bullet,
};

template<typename T>
concept IsComponent = std::derived_from<T, Component>;

template<typename T>
concept IsSprite = std::derived_from<T, Sprite>;

class Entity {

    ComponentTuple             m_components;
    bool                       m_alive;
    size_t                     m_id;
    EntityType                 m_entityType;
    std::shared_ptr<Sprite>    m_sprite;

    Entity(const EntityType entityType, const size_t id) : m_alive(true), m_id(id),
                                                               m_entityType(entityType) {
    };

public:

    template<IsComponent T>
    T& Get() {
        return std::get<T>(m_components);
    }

    template<IsComponent T>
    bool Has() const {
        return std::get<T>(m_components).exists;
    }

    template<IsComponent T, typename ...Args>
    void AddComponent(Args&& ...mArgs) {
        auto& c = std::get<T>(m_components);
        c = T(std::forward<Args>(mArgs)...);
        c.exists = true;
    }

    template<IsSprite T, typename... Args>
    void AddSprite(Args&& ...mArgs) {
        m_sprite = std::make_shared<T>(std::forward<Args>(mArgs)...);
    }

    std::shared_ptr<Sprite> GetSprite() {
        return m_sprite;
    }

    [[nodiscard]] size_t GetId() const { return m_id; }
    [[nodiscard]] bool IsAlive() const { return m_alive; }
    [[nodiscard]] EntityType GetEntityType() const { return m_entityType; }
    void Destroy() { m_alive = false; }
    void draw(const Magnum::Vector2& windowDimensions) {
        if (!m_alive) return;
        const auto sprite = GetSprite();
        const auto transform = Get<Transform>();
        const auto shader = sprite->GetShader();
        shader
            ->setColor(sprite->getColor())
            .setTransformationProjectionMatrix(
                Magnum::Matrix3::projection(windowDimensions)
                * Magnum::Matrix3::translation(transform.position)
                * Magnum::Matrix3::scaling(transform.scale)
            )
            .draw(*sprite->GetMeshData().get());
    }

    friend class EntityManager;
};



#endif //CHURCHILLMAGNUM_ENTITY_H
