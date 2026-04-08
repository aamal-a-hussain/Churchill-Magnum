//
// Created by aamalh on 4/2/26.
//

#ifndef CHURCHILLMAGNUM_ENTITYMANAGER_H
#define CHURCHILLMAGNUM_ENTITYMANAGER_H
#include <map>
#include <memory>

#include "Entity.hpp"
#include "Corrade/Containers/GrowableArray.h"
#include <Corrade/Containers/Array.h>


using EntityVector= Corrade::Containers::Array<std::shared_ptr<Entity>>;
using EntityMap = std::map<EntityType, EntityVector>;

class EntityManager {
    EntityVector    m_entities;
    EntityMap       m_entitiesById;
    EntityVector    m_toAdd;
    size_t          m_totalEntities;

public:
    explicit EntityManager(const size_t num_reserved_entities) {
        Corrade::Containers::arrayReserve(m_entities, num_reserved_entities);
        Corrade::Containers::arrayReserve(m_toAdd, num_reserved_entities);

        m_totalEntities = 0;
    };

    std::shared_ptr<Entity> addEntity(const EntityType entityType) {
        std::shared_ptr<Entity> e_ptr(new Entity(entityType, m_totalEntities++));
        Corrade::Containers::arrayAppend(m_entities, e_ptr);
        Corrade::Containers::arrayAppend(m_entitiesById[entityType], e_ptr);
        return e_ptr;
    }

    EntityVector& getEntity(const EntityType entityType)
    {
        return m_entitiesById[entityType];
    }

    void DrawEntities(const Magnum::Vector2& windowDimensions) {
        if (windowDimensions.x() < 0.0f)
            Corrade::Utility::Fatal {} << "Window size must be greater than zero.";
        for (const auto& entity : m_entities) {
            entity->draw(windowDimensions);
        }

    }
};


#endif //CHURCHILLMAGNUM_ENTITYMANAGER_H