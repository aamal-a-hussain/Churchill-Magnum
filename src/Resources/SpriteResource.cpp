#include "Resources/SpriteResource.h"

SpriteResource::SpriteResource() {
    this->mesh = Magnum::MeshTools::compile(
        Magnum::Primitives::planeSolid(Magnum::Primitives::PlaneFlag::TextureCoordinates));

    // IDLE
    this->loadSprites({SpriteType::IDLE_1, SpriteType::IDLE_2, SpriteType::IDLE_3,
                       SpriteType::IDLE_4, SpriteType::IDLE_5, SpriteType::IDLE_6},
                      TextureLoader::TextureId::IDLE);

    // ATTACK
    this->loadSprites({SpriteType::ATTACK_1, SpriteType::ATTACK_2, SpriteType::ATTACK_3,
                       SpriteType::ATTACK_4, SpriteType::ATTACK_5},
                      TextureLoader::TextureId::ATTACK);
    // RUN
    this->loadSprites(
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
    this->loadSprites(
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


void SpriteResource::loadSprites(std::vector<SpriteType> spriteTypes,
                                 TextureLoader::TextureId textureId) {
    auto info = this->textureLoader.getTextureInfo(textureId);
    auto scale = Magnum::Vector2{1.0f / static_cast<float>(info.n_frames), 1.0f};

    CORRADE_ASSERT(spriteTypes.size() == info.n_frames,
                   std::format("Provided number of sprite types {} does not "
                               "match the number of frames in the texture {}",
                               spriteTypes.size(), info.n_frames)
                       .c_str(), );

    for (size_t col = 0; col < info.n_frames; ++col) {
        auto spriteType = spriteTypes[col];
        Magnum::Vector2 translation = {col * scale.x(), 0};
        CORRADE_ASSERT(!this->sprite_data.contains(spriteType),
                       std::format("{} is being overwritten in the sprite data map",
                                   static_cast<int>(spriteType))
                           .c_str(), );
        this->sprite_data[spriteTypes[col]] = {
            textureId, Magnum::Matrix3::translation(translation) * Magnum::Matrix3::scaling(scale)};
    }
}
