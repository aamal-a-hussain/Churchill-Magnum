//
// Created by aamal on 09/04/2026.
//

#ifndef CHURCHILLMAGNUM_GAME_H
#define CHURCHILLMAGNUM_GAME_H

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/Text/GlyphCacheGL.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "../Entities/EntityManager.hpp"
#include "../GameComponents.hpp"
#include "../Entities/EnemyFab.hpp"
#include "Magnum/PixelFormat.h"
#include "Magnum/Text/AbstractFont.h"
#include "Magnum/Text/RendererGL.h"

#include "Corrade/PluginManager/Manager.h"
#include <Magnum/Shaders/VectorGL.h>



class GameEngine : public Magnum::Platform::Application
{
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

    struct UIProperties
    {
        Magnum::Vector2 position = {-0.9f, 0.95f};
        float size = 1E-3f;
        Magnum::Color3 color = Magnum::Color3::blue();
    };

    virtual ~GameEngine() = default;
    explicit GameEngine(const Arguments &arguments);
    void spawnPlayer();
    void spawnEnemy(EnemyFab& enemyFab);
    void spawnSmall(const std::shared_ptr<Entity>& enemy);
    void spawnBullet(const Magnum::Vector2& position, const Magnum::Vector2& direction);
    void clearEntity(EntityType type);

    static float viewportAspect();
    Magnum::Vector2 windowDimensions();

    static constexpr size_t maxNumEnemies = 8;


private:


    void sEnemySpawner();
    void sCollision();
    void sImGui();
    void sMovement();
    void sMovementInput(const KeyEvent& event, int pressOrRelease);
    void sShoot();
    void sShootInput(int pressOrRelease);
    void incrementScore();
    void decrementHealth();
    void sLifespan();

    void integratePlayerPosition(Transform& t);
    void integratePosition(Transform& t);
    static float rand();
    std::shared_ptr<Entity> player();


    void drawEvent() override;

    void tickEvent() override;

    void viewportEvent(ViewportEvent &event) override;

    void keyPressEvent(KeyEvent &event) override;

    void keyReleaseEvent(KeyEvent &event) override;

    void pointerPressEvent(PointerEvent& event) override;

    void pointerReleaseEvent(PointerEvent& event) override;

    void pointerMoveEvent(PointerMoveEvent& event) override;

    void scrollEvent(ScrollEvent& event) override;

    void textInputEvent(TextInputEvent& event) override;

    EntityManager m_entityManager;
    int8_t m_score = 0;
    int8_t m_health = 100;

    std::shared_ptr<Magnum::Shaders::FlatGL2D> m_texturedShader;
    std::shared_ptr<Magnum::Shaders::FlatGL2D> m_flatShader;
    Magnum::Shaders::VectorGL2D m_text_shader;
    Magnum::PluginManager::Manager<Magnum::Text::AbstractFont> m_manager;
    Corrade::Containers::Pointer<Magnum::Text::AbstractFont> m_font;
    Magnum::Text::GlyphCacheGL m_cache{Magnum::PixelFormat::R8Unorm, {512, 512}};
    Magnum::Text::RendererGL m_score_renderer {m_cache};
    Magnum::Text::RendererGL m_health_renderer {m_cache};

    WindowProperties m_windowProperties;
    PlayerControls m_playerControls;
    UIProperties m_scoreUIProperties = {{-0.9f, 0.95}, 1E-3f, Magnum::Color3::cyan()};
    UIProperties m_healthUIProperties = {{0.9f, 0.95f}, 1E-3f, Magnum::Color3::magenta()};
    Magnum::ImGuiIntegration::Context m_imguiContext {Corrade::NoCreate};


    Magnum::Timeline m_timeline;
    float m_lastSpawnTime = 0.0f;
    float m_spawnInterval = 1.0f;
    float m_lastShotTime = 0.0f;
    Magnum::Vector2i m_pointerLoc{};

    EnemyFab m_runnerFab = EnemyFab::Runner();
    EnemyFab m_heavyFab = EnemyFab::Heavy();
    BulletFab m_bulletFab {};
    float m_collisionCapsuleScale = 0.2f;

    bool m_paused = false;
    bool m_movementActive = true;
    bool m_renderingActive = true;
    bool m_spawnActive = true;
    bool m_shootActive = true;
    bool m_collisionActive = true;
    bool m_lifespanActive = true;
};


#endif //CHURCHILLMAGNUM_GAME_H
