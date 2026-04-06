#include <iostream>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include "Magnum/Text/GlyphCacheGL.h"
#include "Magnum/Text/RendererGL.h"
#include "Corrade/Containers/GrowableArray.h"

#include <Magnum/ImGuiIntegration/Context.hpp>

#include "Entities/EntityManager.h"
#include "Components/GameComponents.hpp"
#include "Components/Sprite.h"


class GameEngine : public Magnum::Platform::Application {
public:
    virtual ~GameEngine() = default;
    explicit GameEngine(const Arguments &arguments);

    static float viewportAspect();

    static constexpr size_t MAX_NUM_SHAPES = 8;

private:
    void drawEvent() override;

    void tickEvent() override;

    void viewportEvent(ViewportEvent &event);

    void keyPressEvent(KeyEvent &event);

    void keyReleaseEvent(KeyEvent &event);

    void pointerPressEvent(PointerEvent &event);

    void pointerReleaseEvent(PointerEvent &event);

    void pointerMoveEvent(PointerMoveEvent &event);

    void scrollEvent(ScrollEvent &event);

    void textInputEvent(TextInputEvent &event);

    EntityManager m_entityManager;
    std::shared_ptr<Magnum::Shaders::FlatGL2D> m_shader;
};


GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
    Configuration{}
    .setTitle("Cwk 1")
    .setWindowFlags(Configuration::WindowFlag::FullscreenDesktop)
    ),
     m_entityManager(1) {

    std::shared_ptr<Entity> player = m_entityManager.addEntity(Player);
    m_shader = std::make_shared<Magnum::Shaders::FlatGL2D>();

    std::cout << player->GetId() << std::endl;
    player->AddComponent<Transform>(
        Magnum::Vector2{0.0f, 0.0f},
        Magnum::Vector2{0.1f, 0.1f},
        Magnum::Vector2{0.0f, 0.0f},
        0.0f
        );

    player->AddSprite<CircleSprite>(m_shader, 0.1f, Magnum::Color3::green(), 8);

    std::cout << "Player Initialised ..." << std::endl;
}

inline float GameEngine::viewportAspect() {
    auto fbSize = Magnum::GL::defaultFramebuffer.viewport().size();
    return float(fbSize.x()) / float(fbSize.y());
}

void GameEngine::drawEvent() {
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);
    m_entityManager.DrawEntities(10.0f, viewportAspect());
    swapBuffers();
}

void GameEngine::tickEvent() {
}

void GameEngine::viewportEvent(ViewportEvent& event) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

}

void GameEngine::keyPressEvent(KeyEvent& event) {
    if (event.key() == Key::Esc) {
        exit(EXIT_SUCCESS);
    }
}

void GameEngine::keyReleaseEvent(KeyEvent& event) {

}

void GameEngine::pointerPressEvent(PointerEvent& event) {

}

void GameEngine::pointerReleaseEvent(PointerEvent& event) {
}

void GameEngine::pointerMoveEvent(PointerMoveEvent& event) {
}

void GameEngine::scrollEvent(ScrollEvent& event) {
}

void GameEngine::textInputEvent(TextInputEvent& event) {
}

MAGNUM_APPLICATION_MAIN(GameEngine);
