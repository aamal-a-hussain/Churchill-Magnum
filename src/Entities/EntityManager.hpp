#ifndef INCLUDE_ENTITIES_ENTITYMANAGER_HPP_
#define INCLUDE_ENTITIES_ENTITYMANAGER_HPP_

#include "Entity.hpp"
class EntityManager {
  // @NOTE:
  // I'm going to try and write this with the following assumption:
  //    1. The first entity is always the player, and must always exist
  std::vector<Entity> m_entities;

public:
  EntityManager() { m_entities.emplace_back(); }

  Entity &getPlayer() { return m_entities[0]; }
};

#endif // INCLUDE_ENTITIES_ENTITYMANAGER_HPP_
