#include <chrono>
#include <thread>

#include "Renderer.h"
#include "Utilities/Profiling.hpp"

Renderer::Renderer() :
    shader{Magnum::Shaders::FlatGL2D::Configuration{}.setFlags(
        Magnum::Shaders::FlatGL2D::Flag::Textured |
        Magnum::Shaders::FlatGL2D::Flag::TextureTransformation)} {}

void Renderer::draw(Sprite& s, Transform& t) {
    PROFILE_FUNCTION

    const auto [textureId, textureMatrix] = this->spriteResource.getData(s.type);
    Magnum::GL::Texture2D& texture = spriteResource.textureLoader.get(textureId);
    const auto& [pos, scale, rot] = t;
    this->shader.bindTexture(texture)
        .setTextureMatrix(textureMatrix)
        .setTransformationProjectionMatrix(Magnum::Matrix3::translation(pos) *
                                           Magnum::Matrix3::scaling(scale) *
                                           Magnum::Matrix3::rotation(Magnum::Rad(rot)))
        .draw(spriteResource.getMesh());
    std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate a long draw call
}
