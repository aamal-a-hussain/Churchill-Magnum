#ifndef INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
#define INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_

#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/Trade/MeshData.h>

#include <map>

class SpriteResource {
public:
  enum class SpriteType { FALLBACK };

  SpriteResource() {
    m_sprites[SpriteType::FALLBACK] =
        Magnum::MeshTools::compile(Magnum::Primitives::squareSolid());
  }

  Magnum::GL::Mesh &get(SpriteType spriteType) {
    return m_sprites.at(spriteType);
  }

private:
  std::map<SpriteType, Magnum::GL::Mesh> m_sprites;
};

#endif // INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
