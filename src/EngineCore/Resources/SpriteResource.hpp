#ifndef INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
#define INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_

#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Plane.h>
#include <Magnum/Trade/MeshData.h>

#include <unordered_map>

class SpriteResource {
public:
  enum class SpriteType { FALLBACK, IDLE_1 };

  SpriteResource() {
    m_sprites[SpriteType::FALLBACK] =
        Magnum::MeshTools::compile(Magnum::Primitives::planeSolid(
            Magnum::Primitives::PlaneFlag::TextureCoordinates));
    m_sprites[SpriteType::IDLE_1] = Magnum::MeshTools::compile()
  }

  Magnum::GL::Mesh &get(SpriteType spriteType) {
    return m_sprites.at(spriteType);
  }

private:
  std::unordered_map<SpriteType, Magnum::GL::Mesh> m_sprites;
};

#endif // INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
