#include <iostream>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Text/RendererGL.h>

#include "Game.h"

GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
    Configuration{}
    .setTitle("Cwk 2")
    .setWindowFlags(Configuration::WindowFlag::FullscreenDesktop)
    ),
    m_entityManager(maxNumEnemies + 1) {
    m_texturedShader = std::make_shared<Magnum::Shaders::FlatGL2D>(Magnum::Shaders::FlatGL2D::Flag::Textured);
    m_flatShader = std::make_shared<Magnum::Shaders::FlatGL2D>();

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
    }
    sImGui();
    swapBuffers();
}


void GameEngine::tickEvent() {
    const auto cTime = m_timeline.currentFrameTime();
    if (!m_paused) {
        m_entityManager.update();
        if (m_movementActive) sMovement();
        if (m_spawnActive) sSpawnEnemy();

        for (const auto& e : m_entityManager.getEntityById(EntityType::Enemy))
        {
            if (e->Has<LifeSpan>())
                if (const auto& l = e->Get<LifeSpan>(); cTime - l.startTime >= l.lifespan) {
                    e->Destroy();
                }
        }
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
    sHandleInput(event, 1);
}

void GameEngine::keyReleaseEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyReleaseEvent(event)) return;
    sHandleInput(event, 0);
}

void GameEngine::pointerPressEvent(PointerEvent& event)
{
    if(m_imguiContext.handlePointerPressEvent(event)) return;
}

void GameEngine::pointerReleaseEvent(PointerEvent& event) {
    if(m_imguiContext.handlePointerReleaseEvent(event)) return;
}

void GameEngine::pointerMoveEvent(PointerMoveEvent& event) {
    if(m_imguiContext.handlePointerMoveEvent(event)) return;
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
