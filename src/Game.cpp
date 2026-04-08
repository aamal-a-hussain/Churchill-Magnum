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
    struct PlayerControls
    {
        float movementSpeed = 4.0f;
        float warpSpeed = 15.0f;
    };

    struct WindowProperties
    {
        float windowSize = 10.0f;
    };

    virtual ~GameEngine() = default;
    explicit GameEngine(const Arguments &arguments);

    static float viewportAspect();
    Magnum::Vector2 windowDimensions();

    static constexpr size_t MAX_NUM_SHAPES = 8;


private:
    void drawEvent() override;

    void tickEvent() override;

    void viewportEvent(ViewportEvent &event) override;
    void sImGui();
    void sMovement();
    std::shared_ptr<Entity> player();
    void sHandleInput(const KeyEvent& event, int pressOrRelease);

    void keyPressEvent(KeyEvent &event) override;

    void keyReleaseEvent(KeyEvent &event) override;

    void pointerPressEvent(PointerEvent& event) override;

    void pointerReleaseEvent(PointerEvent& event) override;

    void pointerMoveEvent(PointerMoveEvent& event) override;

    void scrollEvent(ScrollEvent& event) override;

    void textInputEvent(TextInputEvent& event) override;

    EntityManager m_entityManager;
    std::shared_ptr<Magnum::Shaders::FlatGL2D> m_shader;
    Magnum::Timeline m_timeline;
    Magnum::ImGuiIntegration::Context m_imguiContext {Corrade::NoCreate};
    WindowProperties m_windowProperties;
    PlayerControls m_playerControls;

};


GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
    Configuration{}
    .setTitle("Cwk 2")
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
    player->AddComponent<Input>();

    std::cout << "Player Initialised ..." << std::endl;
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

std::shared_ptr<Entity> GameEngine::player()
{
    return m_entityManager.getEntity(Player).back();
}

void GameEngine::drawEvent() {
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);
    m_entityManager.DrawEntities(windowDimensions());
    sImGui();
    swapBuffers();
}

void GameEngine::tickEvent() {
    sMovement();

    m_timeline.nextFrame();
    redraw();
}

void GameEngine::viewportEvent(ViewportEvent& event) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

}

void GameEngine::sImGui()
{
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::ScissorTest);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

    m_imguiContext.newFrame();

    if (ImGui::CollapsingHeader("Window Controls"))
    {
        ImGui::SliderFloat("Window Size", &m_windowProperties.windowSize, 1.0f, 20.0f);
    }
    if (ImGui::CollapsingHeader("Player Controls"))
    {
        ImGui::SliderFloat("Movement Speed", &m_playerControls.movementSpeed, 1.0f, 10.0f);
        ImGui::SliderFloat("Warp Speed", &m_playerControls.warpSpeed, 5.0f, 20.0f);
    }

    m_imguiContext.updateApplicationCursor(*this);


    m_imguiContext.drawFrame();
}



void GameEngine::sMovement()
{
    const float dt = m_timeline.currentFrameDuration();
    auto& t = player()->Get<Transform>();
    auto& i = player()->Get<Input>();

    Magnum::Vector2 newVelocity {};

    if (i.up) newVelocity.y() += 1.0f;
    if (i.down) newVelocity.y() -= 1.0f;
    if (i.left) newVelocity.x() -= 1.0f;
    if (i.right) newVelocity.x() += 1.0f;

    newVelocity /= newVelocity.length() + 1E-8f;

    const auto newSpeed = i.warp ? m_playerControls.warpSpeed : m_playerControls.movementSpeed;
    newVelocity *= newSpeed;

    t.velocity = newVelocity;

    Magnum::Vector2 newPosition {t.position.x(), t.position.y()};

    newPosition.x() += t.velocity.x() * dt;
    newPosition.y() += t.velocity.y() * dt;

    if (newPosition.x() - t.scale.x() / 2 < -windowDimensions().x() / 2 || newPosition.x() + t.scale.x() / 2 > windowDimensions().x() / 2) newPosition.x() = t.position.x();
    if (newPosition.y() - t.scale.y() / 2 < -windowDimensions().y() / 2 || newPosition.y() + t.scale.y() / 2 > windowDimensions().y() / 2) newPosition.y() = t.position.y();


    t.position = newPosition;
}

void GameEngine::sHandleInput(const KeyEvent &event, int pressOrRelease)
{
    auto& i = player()->Get<Input>();
    if (event.key() == Key::W) i.up = pressOrRelease;
    if (event.key() == Key::S) i.down = pressOrRelease;
    if (event.key() == Key::A) i.left = pressOrRelease;
    if (event.key() == Key::D) i.right = pressOrRelease;
    if (event.key() == Key::Space) i.warp = pressOrRelease; // @TODO: change this to use mouse click
}

void GameEngine::keyPressEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyPressEvent(event)) return;
    if (event.key() == Key::Esc) {
        exit(EXIT_SUCCESS);
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
