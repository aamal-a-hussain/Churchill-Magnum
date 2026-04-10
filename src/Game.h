//
// Created by aamal on 09/04/2026.
//

#ifndef CHURCHILLMAGNUM_GAME_H
#define CHURCHILLMAGNUM_GAME_H

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/Text/GlyphCacheGL.h>
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
    void spawnPlayer();
    void spawnEnemy(EnemyFab& enemyFab);
    void clearEnemies();

    static float viewportAspect();
    Magnum::Vector2 windowDimensions();

    static constexpr size_t maxNumEnemies = 8;


private:


    void sSpawnEnemy();
    void sImGui();
    void sMovement();
    void sHandleInput(const KeyEvent& event, int pressOrRelease);

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

    std::shared_ptr<Magnum::Shaders::FlatGL2D> m_shader;
    Magnum::Timeline m_timeline;
    Magnum::ImGuiIntegration::Context m_imguiContext {Corrade::NoCreate};
    WindowProperties m_windowProperties;
    PlayerControls m_playerControls;
    float m_lastSpawnTime = 0.0f;
    float m_spawnInterval = 1.0f;

    EnemyFab m_runnerFab = EnemyFab::Runner();
    EnemyFab m_heavyFab = EnemyFab::Heavy();

    bool m_paused = false;
    bool m_movementActive = true;
    bool m_renderingActive = true;
    bool m_spawnActive = true;
};


#endif //CHURCHILLMAGNUM_GAME_H