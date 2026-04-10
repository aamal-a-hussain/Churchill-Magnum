#include <iostream>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Text/RendererGL.h>
#include <Magnum/Text/AbstractShaper.h>


#include "GameEngine.h"

GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
                                                         Configuration{}
                                                         .setTitle("Cwk 2")
                                                         .setWindowFlags(Configuration::WindowFlag::FullscreenDesktop)
                                                     ),
                                                     m_entityManager(maxNumEnemies + 1) {
    m_texturedShader = std::make_shared<Magnum::Shaders::FlatGL2D>(Magnum::Shaders::FlatGL2D::Flag::Textured);
    m_flatShader = std::make_shared<Magnum::Shaders::FlatGL2D>();

    m_font = m_manager.loadAndInstantiate("StbTrueTypeFont");
    const Corrade::Utility::Resource rs{"ui-data"};
    if (!m_font->openData(rs.getRaw("ui_font.ttf"), 32.0f))
        Corrade::Utility::Fatal {} << "Could not load font";

    if (!m_font->fillGlyphCache(m_cache,
                        "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"))
        Corrade::Utility::Fatal {} << "Glyph cache fill failed";

    m_fontShaper = m_font->createShaper();
    m_fontSize = m_font->size();

    m_score_renderer.reserve(100, (Magnum::UnsignedInt)Magnum::GL::BufferUsage::DynamicDraw);
    m_score_renderer.render(*m_fontShaper, m_fontSize, "Score: 0");

    m_health_renderer.reserve(100, (Magnum::UnsignedInt)Magnum::GL::BufferUsage::DynamicDraw);
    m_health_renderer.render(*m_fontShaper, m_fontSize, "Health: 100");

    spawnPlayer();

    m_timeline.start();
    m_imguiContext = Magnum::ImGuiIntegration::Context{
        Magnum::Vector2{windowSize()} / dpiScaling(), windowSize(), framebufferSize()
    };

    Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add,
        Magnum::GL::Renderer::BlendEquation::Add);
    Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha,
        Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
}


inline float GameEngine::viewportAspect() {
    auto fbSize = Magnum::GL::defaultFramebuffer.viewport().size();
    return float(fbSize.x()) / float(fbSize.y());
}

inline Magnum::Vector2 GameEngine::windowDimensions()
{
    return {m_windowProperties.windowSize * viewportAspect(), m_windowProperties.windowSize};
}



void GameEngine::drawEvent() {
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);
    if (m_renderingActive) {
        m_entityManager.DrawEntities(windowDimensions());
        m_text_shader
            .setColor(m_scoreUIProperties.color)
            .setTransformationProjectionMatrix(
                Magnum::Matrix3::translation(m_scoreUIProperties.position)
                * Magnum::Matrix3::scaling(Magnum::Vector2{m_scoreUIProperties.size})
                )
            .bindVectorTexture(m_cache.texture())
            .draw(m_score_renderer.mesh());

        m_text_shader
            .setColor(m_healthUIProperties.color)
            .setTransformationProjectionMatrix(
                Magnum::Matrix3::translation(m_healthUIProperties.position)
                * Magnum::Matrix3::scaling(Magnum::Vector2{m_healthUIProperties.size})
                )
            .bindVectorTexture(m_cache.texture())
            .draw(m_health_renderer.mesh());
    }

    sImGui();
    swapBuffers();
}


void GameEngine::tickEvent() {
    if (!m_paused) {
        m_entityManager.update();
        if (m_movementActive) sMovement();
        if (m_spawnActive) sEnemySpawner();
        if (m_shootActive) sShoot();
        if (m_collisionActive) sCollision();
        if (m_lifespanActive) sLifespan();

    }
    m_timeline.nextFrame();
    redraw();
}

void GameEngine::viewportEvent(ViewportEvent& event) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

}

void GameEngine::keyPressEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyPressEvent(event)) return;
    if (event.key() == Key::Esc) {
        m_paused = !m_paused;
    }
    sMovementInput(event, 1);
}

void GameEngine::keyReleaseEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyReleaseEvent(event)) return;
    sMovementInput(event, 0);
}


void GameEngine::pointerPressEvent(PointerEvent& event)
{
    if(m_imguiContext.handlePointerPressEvent(event)) return;
    sShootInput(1);
}

void GameEngine::pointerReleaseEvent(PointerEvent& event) {
    if(m_imguiContext.handlePointerReleaseEvent(event)) return;
    sShootInput(0);
}

void GameEngine::pointerMoveEvent(PointerMoveEvent& event) {
    if(m_imguiContext.handlePointerMoveEvent(event)) return;
    m_pointerLoc = {(int) event.position().x(), (int) event.position().y()};
}

void GameEngine::scrollEvent(ScrollEvent& event) {
    if(m_imguiContext.handleScrollEvent(event)) {
        event.setAccepted();
        return;
    }
}

void GameEngine::textInputEvent(TextInputEvent& event) {
    if(m_imguiContext.handleTextInputEvent(event)) return;
}

MAGNUM_APPLICATION_MAIN(GameEngine);
