#include "Components/Transform.h"
#include "EngineCore/Config.h"
#include "EngineCore/Engine.h"
#include "Magnum/GL/AbstractFramebuffer.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/GL/Renderer.h"
#include "Magnum/Magnum.h"

// @TODO: Remove the hardcoded values;
GameEngine::GameEngine(const Arguments& arguments) :
    Magnum::Platform::Application(
        arguments,
        Configuration{}
            .setTitle(game::config::NAME)
            .setSize({game::config::WINDOW_WIDTH, game::config::WINDOW_HEIGHT})),
    renderer(), entityManager() {

    // Needed for IMGUI to work.
    namespace GL = Magnum::GL;
    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
                                   GL::Renderer::BlendEquation::Add);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
                                   GL::Renderer::BlendFunction::OneMinusSourceAlpha);
}

void GameEngine::drawEvent() {
    namespace GL = Magnum::GL;
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    static Sprite sprite{SpriteResource::SpriteType::ATTACK_5};
    static Transform transform{.position = {0.0f, 0.0f}, .scale = {1.0f, 1.0f}, .rotation = 0.0f};

    this->renderer.draw(sprite, transform);

    swapBuffers();
}

void GameEngine::tickEvent() { redraw(); }

void GameEngine::keyPressEvent(KeyEvent& event) {
    if (event.key() == Key::Esc)
        exit();
    this->inputSystem.handleKeyDown(event);
}

MAGNUM_APPLICATION_MAIN(GameEngine);
