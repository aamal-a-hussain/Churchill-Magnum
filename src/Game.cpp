#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/GL/Mesh.h>
#include <Corrade/Containers/Array.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Timeline.h>

#include <Magnum/ImGuiIntegration/Context.hpp>


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

    Magnum::ImGuiIntegration::Context m_imguiContext {Corrade::NoCreate};
    Corrade::Containers::StaticArray<MAX_NUM_SHAPES, Corrade::Containers::Optional<Magnum::GL::Mesh> > m_meshes;
    Magnum::Shaders::FlatGL2D m_shader;
    Magnum::Timeline m_timeline;
    float m_xValue {};
    Magnum::Color3 m_meshColor {Magnum::Color3::red()};
};


GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
    Configuration{}
    .setTitle("Cwk 1")
    .setSize({1000, 600})
    .setWindowFlags(Configuration::WindowFlag::Resizable)
) {
    m_meshes[0].emplace(Magnum::MeshTools::compile(Magnum::Primitives::circle2DSolid(64)));
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
        ImGui::ColorEdit3("Circle Color", m_meshColor.data());
    }

    for (size_t i = 0; i < MAX_NUM_SHAPES; ++i) {
        if (m_meshes[i])
            m_shader
                    .setColor(m_meshColor)
                    .setTransformationProjectionMatrix(
                        Magnum::Matrix3::projection({5.0f, 3.0f})
                        * Magnum::Matrix3::translation({m_xValue, 0})
                        * Magnum::Matrix3::scaling(Magnum::Vector2{0.1f})
                    )
                    .draw(*m_meshes[i]);
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
    m_xValue = Magnum::Math::sin(Magnum::Math::Rad<float>(0.5f * timeElapsed));

    redraw();
}

void GameEngine::viewportEvent(ViewportEvent& event) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    m_imguiContext.relayout(Magnum::Vector2{event.windowSize()}/event.dpiScaling(),
        event.windowSize(), event.framebufferSize());
}

void GameEngine::keyPressEvent(KeyEvent& event) {
    if(m_imguiContext.handleKeyPressEvent(event)) return;
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
        /* Prevent scrolling the page */
        event.setAccepted();
        return;
    }
}

void GameEngine::textInputEvent(TextInputEvent& event) {
    if(m_imguiContext.handleTextInputEvent(event)) return;
}


MAGNUM_APPLICATION_MAIN(GameEngine);
