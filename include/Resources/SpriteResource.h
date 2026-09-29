#ifndef INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
#define INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_

#include <format>
#include "Corrade/Utility/DebugAssert.h"
#include "Magnum/Magnum.h"
#include "TextureLoader.hpp"

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
        FALLBACK, // Empty sprite in case one fails to load
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

    /**
     * @brief Container for sprite data
     *
     * Contains all the data that should be required to draw the sprite
     * except for the transform
     */
    struct SpriteData {
        TextureLoader::TextureId textureId;
        Magnum::Matrix3 textureMatrix;
    };

    /**
     * @brief Initialise sprites
     *
     * Sets up all of the sprites in the game
     *
     * @aamalh: Probably not the best idea, we probably want to be
     * able to load in the required data for a level on demand.
     */
    SpriteResource();

    /**
     * @brief Get the SpriteData container for a given sprite type
     *
     * @aamalh: This works now since all sprites are initialised at startup
     * but when we load things in on demand, we will need to check if it
     * has been initialised.
     */
    SpriteData& getData(SpriteType spriteType) { return this->sprite_data.at(spriteType); }

    /**
     * @brief Get the mesh for rendering
     *
     * Everything is drawn as a plane with a texture on it, so
     * there only needs to be one mesh (a plane) and the SpriteData
     * can contain the texture id.
     */
    Magnum::GL::Mesh& getMesh() { return this->mesh; }

public:
    TextureLoader textureLoader;

private:
    /**
     * @brief Load a set of sprites from a single texture.
     *
     * The sprites are stored in a sprite sheet, and so come from a single texture,
     * just with varying texture coordinates. Therefore, we assign to each sprite type
     * the same texture id and vary the coordinates, and save the result as a SpriteData object
     *
     * If a sprite fails to load (e.g. if the texture is not loaded), an empty
     * fallback sprite is used.
     *
     *  TODO: Create the fallback sprite
     */
    void loadSprites(std::vector<SpriteType> spriteTypes, TextureLoader::TextureId textureId);

private:
    std::unordered_map<SpriteType, SpriteData> sprite_data;
    Magnum::GL::Mesh mesh;
};

#endif // INCLUDE_ENGINECORE_SPRITERESOURCE_HPP_
