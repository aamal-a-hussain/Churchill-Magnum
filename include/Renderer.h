#ifndef INCLUDE_ENGINECORE_RENDERER_HPP_
#define INCLUDE_ENGINECORE_RENDERER_HPP_

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Vector.h>
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Magnum/GL/Texture.h"
#include "Magnum/Magnum.h"
#include "Magnum/Shaders/FlatGL.h"
#include "Resources/SpriteResource.h"

class Renderer {

public:
    /**
     * @brief Create a renderer
     *
     * Initialises the Sprite Resource, which the renderer holds and
     * creates the single shader (FlatGL2D) for rendering all the sprites
     */
    Renderer();


    /**
     * @brief Draw a sprite at a given transform
     */
    void draw(Sprite& s, Transform& t);

private:
    Magnum::Shaders::FlatGL2D shader;
    SpriteResource spriteResource;
};

#endif // INCLUDE_ENGINECORE_RENDERER_HPP_
