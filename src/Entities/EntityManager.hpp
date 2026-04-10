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

#include "EnemyFab.hpp"


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
    }

    std::shared_ptr<Entity> addEntity(const EntityType entityType) {
        std::shared_ptr<Entity> e_ptr(new Entity(entityType, m_totalEntities++));
        Corrade::Containers::arrayAppend(m_toAdd, e_ptr);
        return e_ptr;

    }

    [[nodiscard]] bool playerExists() const {
        return m_entitiesById.contains(EntityType::Player);
    }

    void update()
    {
        for (auto& e : m_toAdd)
        {
            Corrade::Containers::arrayAppend(m_entities, e);
            Corrade::Containers::arrayAppend(m_entitiesById[e->GetEntityType()], e);
        }

        for (size_t i = 0; i < m_entities.size(); ++i)
        {
            if (const auto& e = m_entities[i]; !e->IsAlive())
                Corrade::Containers::arrayRemove(m_entities, i);
        }

        for (auto& [type, e_vec] : m_entitiesById)
        {
            for (size_t i = 0; i < e_vec.size(); ++i)
            {
                if (const auto& e = e_vec[i]; !e->IsAlive())
                    Corrade::Containers::arrayRemove(m_entitiesById[type], i);
            }
        }

        Corrade::Containers::arrayClear(m_toAdd);
    }

    EntityVector& getEntityById(const EntityType entityType)
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