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
#include <Corrade/Tags.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "SpriteLoader.hpp"
#include "Corrade/PluginManager/Manager.h"
#include "Magnum/PixelFormat.h"
#include "Magnum/Text/AbstractFont.h"
#include "Magnum/Text/GlyphCacheGL.h"
#include "Magnum/Text/RendererGL.h"
#include <Magnum/Text/AbstractShaper.h>

#include "Corrade/Containers/GrowableArray.h"
#include "Magnum/Shaders/VectorGL.h"



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

    size_t m_num_shapes = MAX_NUM_SHAPES;

    size_t m_selectedIndex = 0;
    Magnum::ImGuiIntegration::Context m_imguiContext {Corrade::NoCreate};
    bool m_isPaused = false;

    Magnum::Shaders::FlatGL2D m_shader;
    Magnum::Shaders::VectorGL2D m_text_shader;
    Magnum::Timeline m_timeline;
    Magnum::PluginManager::Manager<Magnum::Text::AbstractFont> m_manager;
    Corrade::Containers::Pointer<Magnum::Text::AbstractFont> m_font;
    Magnum::Text::GlyphCacheGL m_cache{Magnum::PixelFormat::R8Unorm, {512, 512}};

    Corrade::Containers::Array<Magnum::Text::RendererGL> m_text_renderer {};

    Corrade::Containers::Array<Corrade::Containers::Pointer<Sprite>> m_sprites;
    Corrade::Containers::Array<std::string> m_spriteNames {};
    Corrade::Containers::Array<std::string> m_spriteInnerText {};
    Corrade::Containers::Array<bool> m_nameChanged {};
    Corrade::Containers::Array<bool> m_spriteAlive {};
};


GameEngine::GameEngine(const Arguments &arguments) : Magnum::Platform::Application(arguments,
    Configuration{}
    .setTitle("Cwk 1")
    .setSize({800, 800})
) {

    m_font = m_manager.loadAndInstantiate("StbTrueTypeFont");
    m_font->openFile("./res/FSEX300.ttf", 10.0f);

    if (!m_font->fillGlyphCache(m_cache,
                        "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"))
        Corrade::Utility::Fatal {} << "Glyph cache fill failed";

    auto loader = SpriteLoader::readFromFile("./res/sprites.conf");
    m_num_shapes = loader.length();
    Corrade::Containers::arrayReserve(m_sprites, m_num_shapes);
    Corrade::Containers::arrayReserve(m_spriteNames, m_num_shapes);
    Corrade::Containers::arrayReserve(m_spriteInnerText, m_num_shapes);
    Corrade::Containers::arrayReserve(m_text_renderer, m_num_shapes);
    Corrade::Containers::arrayReserve(m_nameChanged, m_num_shapes);
    Corrade::Containers::arrayReserve(m_spriteAlive, m_num_shapes);

    size_t idx = 0;
    for (auto sprite : loader) {
        if (idx >= MAX_NUM_SHAPES)
        {
            Corrade::Utility::Warning {} << "More shapes than allowed. Skipping remaining shapes...";
            break;
        }
        Corrade::Containers::arrayAppend(m_spriteNames, sprite->getName());
        Corrade::Containers::arrayAppend(m_spriteInnerText, sprite->getName());

        auto t_renderer = Magnum::Text::RendererGL {m_cache};
        t_renderer.reserve(100, (Magnum::UnsignedInt)Magnum::GL::BufferUsage::DynamicDraw);
        t_renderer.render(*m_font -> createShaper(), m_font->size(), sprite->getName().c_str());
        Corrade::Containers::arrayAppend(m_text_renderer, std::move(t_renderer));
        Corrade::Containers::arrayAppend(m_nameChanged, false);
        Corrade::Containers::arrayAppend(m_spriteAlive, sprite->isAlive());
        Corrade::Containers::arrayAppend(m_sprites, std::move(sprite));
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
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::ScissorTest);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

    m_imguiContext.newFrame();

    {
        for (size_t idx = 0; idx < m_num_shapes; ++idx)
        {
            if (m_nameChanged[idx])
            {
                m_text_renderer[idx].clear();
                m_text_renderer[idx].render(*m_font->createShaper(), m_font->size(), m_spriteInnerText[idx].c_str());
                m_nameChanged[idx] = false;
            }
            m_sprites[idx]->setAlive(m_spriteAlive[idx]);
        }


        if (ImGui::BeginCombo("Select sprite", m_spriteNames[m_selectedIndex].c_str()))
        {
            for (size_t idx = 0; idx < m_num_shapes; ++idx)
            {
                if (m_spriteNames[idx].empty()) continue;

                const bool isSelected = m_selectedIndex == idx;
                if (ImGui::Selectable(m_spriteNames[idx].c_str(), isSelected))
                {
                    m_selectedIndex = idx;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::ColorEdit3(std::format("Color", m_spriteNames[m_selectedIndex]).c_str(), m_sprites[m_selectedIndex]->colorData());
        if (ImGui::InputText("Inner Text", m_spriteInnerText[m_selectedIndex].data(), 100)) m_nameChanged[m_selectedIndex] = true;
        ImGui::Checkbox("Alive", &m_spriteAlive[m_selectedIndex]);
    }

    for (size_t idx = 0; idx < m_num_shapes; ++idx) {
        if (auto& sprite = m_sprites[idx]; sprite->isAlive() && sprite->hasMesh()) {
        m_shader
                .setColor(sprite->getColor())
                .setTransformationProjectionMatrix(
                    * Magnum::Matrix3::translation(sprite->getTranslation())
                    * Magnum::Matrix3::scaling(sprite->getScale() )
                )
                .draw(sprite->getMesh());

            m_text_shader
                .setTransformationProjectionMatrix(
                    Magnum::Matrix3::projection(Magnum::Vector2 {windowSize()})
                    * Magnum::Matrix3::translation(
                        sprite->getTranslation() * Magnum::Vector2{windowSize()} * 0.5
                        )
                    )
                .bindVectorTexture(m_cache.texture())
                .draw(m_text_renderer[idx].mesh());
        }
    }

    m_imguiContext.updateApplicationCursor(*this);


    m_imguiContext.drawFrame();


    swapBuffers();
}

void GameEngine::tickEvent() {
    const float timeElapsed = m_timeline.currentFrameDuration();
    if (!m_isPaused) {
        for (auto& sprite : m_sprites) {
            if (sprite->isAlive())
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
