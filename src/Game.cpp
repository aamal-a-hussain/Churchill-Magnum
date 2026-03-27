#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Circle.h>
#include <Corrade/Containers/Array.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Timeline.h>

#include <Corrade/Containers/Pointer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "SpriteLoader.h"


class GameEngine : public Magnum::Platform::Application {
public:
    virtual ~GameEngine() = default;

    explicit GameEngine(const Arguments &arguments);

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

    float aspectRatio() const { return (float)windowSize().x() / (float)windowSize().y(); }

    Magnum::ImGuiIntegration::Context m_imguiContext {Corrade::NoCreate};
    Corrade::Containers::StaticArray<
        MAX_NUM_SHAPES, Corrade::Containers::Pointer<Sprite>
    > m_sprites;
    Magnum::Shaders::FlatGL2D m_shader;
    Magnum::Timeline m_timeline;
    bool m_isPaused = false;
};


GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
    Configuration{}
    .setTitle("Cwk 1")
    .setSize({800, 800}) // @TODO allow resizable
) {
    size_t idx = 0;
    for (auto sprite : SpriteLoader::readFromFile("./res/sprites.conf")) {
        if (idx >= MAX_NUM_SHAPES) break;
        m_sprites[idx] = std::move(sprite);
        ++idx;
    }

    m_timeline.start();
    m_imguiContext = Magnum::ImGuiIntegration::Context{
        Magnum::Vector2{windowSize()} / dpiScaling(), windowSize(), framebufferSize()
    };

    Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add,
        Magnum::GL::Renderer::BlendEquation::Add);
    Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha,
        Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
}

void GameEngine::drawEvent() {
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    m_imguiContext.newFrame();

    {
        size_t idx = 0;
        for (auto &sprite : m_sprites) {
            if (sprite && sprite->hasMesh()) {
                ImGui::ColorEdit3(std::format("Sprite {} color", idx++).c_str(), sprite->colorData());

                m_shader
                    .setColor(sprite->getColor())
                    .setTransformationProjectionMatrix(
                        Magnum::Matrix3::translation(sprite->getTranslation())
                        * Magnum::Matrix3::scaling(sprite->getScale())
                    )
                    .draw(sprite->getMesh());
            }
        }
    }

    m_imguiContext.updateApplicationCursor(*this);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::ScissorTest);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

    m_imguiContext.drawFrame();

    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::ScissorTest);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::Blending);

    swapBuffers();
}

void GameEngine::tickEvent() {
    const float timeElapsed = m_timeline.currentFrameDuration();
    if (!m_isPaused) {
        for (auto& sprite : m_sprites) {
            if (sprite)
                sprite->step(timeElapsed);
        }
    }
    m_timeline.nextFrame();
    redraw();
}

void GameEngine::viewportEvent(ViewportEvent& event) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    m_imguiContext.relayout(Magnum::Vector2{event.windowSize()}/event.dpiScaling(),
        event.windowSize(), event.framebufferSize());
}

void GameEngine::keyPressEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyPressEvent(event)) return;
    if (event.key() == Key::Space) {
        m_isPaused = !m_isPaused;
    }
}

void GameEngine::keyReleaseEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyReleaseEvent(event)) return;
}

void GameEngine::pointerPressEvent(PointerEvent& event) {
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
