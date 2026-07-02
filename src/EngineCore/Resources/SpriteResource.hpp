#ifndef INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
#define INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_

#include "Corrade/Utility/DebugAssert.h"
#include "Magnum/Magnum.h"
#include "TextureLoader.hpp"
#include <format>

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Plane.h>
#include <Magnum/Trade/MeshData.h>

#include <unordered_map>
#include <vector>

class SpriteResource {
public:
  enum class SpriteType {
    FALLBACK,
    IDLE_1,
    IDLE_2,
    IDLE_3,
    IDLE_4,
    IDLE_5,
    IDLE_6,
    ATTACK_1,
    ATTACK_2,
    ATTACK_3,
    ATTACK_4,
    ATTACK_5,
    RUN_1,
    RUN_2,
    RUN_3,
    RUN_4,
    RUN_5,
    RUN_6,
    RUN_7,
    RUN_8,
    WALK_1,
    WALK_2,
    WALK_3,
    WALK_4,
    WALK_5,
    WALK_6,
    WALK_7,
    WALK_8,
  };
  struct SpriteData {
    TextureLoader::TextureId textureId;
    Magnum::Matrix3 textureMatrix;
  };

  SpriteResource() {
    m_mesh = Magnum::MeshTools::compile(Magnum::Primitives::planeSolid(
        Magnum::Primitives::PlaneFlag::TextureCoordinates));

    // IDLE
    _loadSprite({SpriteType::IDLE_1, SpriteType::IDLE_2, SpriteType::IDLE_3,
                 SpriteType::IDLE_4, SpriteType::IDLE_5, SpriteType::IDLE_6},
                TextureLoader::TextureId::IDLE);

    // ATTACK
    _loadSprite({SpriteType::ATTACK_1, SpriteType::ATTACK_2,
                 SpriteType::ATTACK_3, SpriteType::ATTACK_4,
                 SpriteType::ATTACK_5},
                TextureLoader::TextureId::ATTACK);
    // RUN
    _loadSprite(
        {
            SpriteType::RUN_1,
            SpriteType::RUN_2,
            SpriteType::RUN_3,
            SpriteType::RUN_4,
            SpriteType::RUN_5,
            SpriteType::RUN_6,
            SpriteType::RUN_7,
            SpriteType::RUN_8,
        },
        TextureLoader::TextureId::RUN);
    // WALK
    _loadSprite(
        {
            SpriteType::WALK_1,
            SpriteType::WALK_2,
            SpriteType::WALK_3,
            SpriteType::WALK_4,
            SpriteType::WALK_5,
            SpriteType::WALK_6,
            SpriteType::WALK_7,
            SpriteType::WALK_8,
        },
        TextureLoader::TextureId::WALK);
  }

  SpriteData &getData(SpriteType spriteType) {
    return m_sprite_data.at(spriteType);
  }

  Magnum::GL::Mesh &getMesh() { return m_mesh; }
  TextureLoader textureLoader;

private:
  std::unordered_map<SpriteType, SpriteData> m_sprite_data;
  Magnum::GL::Mesh m_mesh;

  void _loadSprite(std::vector<SpriteType> spriteTypes,
                   TextureLoader::TextureId textureId) {
    auto info = textureLoader.getTextureInfo(textureId);
    auto scale =
        Magnum::Vector2{1.0f / static_cast<float>(info.n_frames), 1.0f};

    CORRADE_ASSERT(spriteTypes.size() == info.n_frames,
                   std::format("Provided number of sprite types {} does not "
                               "match the number of frames in the texture {}",
                               spriteTypes.size(), info.n_frames)
                       .c_str(), );

    for (std::size_t col = 0; col < info.n_frames; ++col) {
      auto spriteType = spriteTypes[col];
      Magnum::Vector2 translation = {col * scale.x(), 0};
      CORRADE_ASSERT(
          !m_sprite_data.contains(spriteType),
          std::format("{} is being overwritten in the sprite data map",
                      static_cast<int>(spriteType))
              .c_str(), );
      m_sprite_data[spriteTypes[col]] = {
          textureId, Magnum::Matrix3::translation(translation) *
                         Magnum::Matrix3::scaling(scale)};
    }
  }
};

#endif // INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
