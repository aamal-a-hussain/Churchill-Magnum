//
// Created by aamal on 09/04/2026.
//

#include <Magnum/GL/Renderer.h>

#include "Game.h"

std::shared_ptr<Entity> GameEngine::player()
{
    return m_entityManager.getEntityById(Player).back();
}

void GameEngine::spawnPlayer()
{
    std::shared_ptr<Entity> player = m_entityManager.addEntity(Player);
    player->AddComponent<Transform>(
        Magnum::Vector2{0.0f, 0.0f},
        Magnum::Vector2{0.1f, 0.1f},
        Magnum::Vector2{0.0f, 0.0f},
        0.0f
        );

    player->AddSprite<CircleSprite>(m_shader, 0.1f, Magnum::Color3::green(), 8);
    player->AddComponent<Input>();
}

void GameEngine::spawnEnemy(EnemyFab& enemyFab)
{
    const std::shared_ptr<Entity> enemy = m_entityManager.addEntity(Enemy);

    Magnum::Vector2 velocity {0.1f, 2.0f};
    velocity /= velocity.length() + 1E-8f;
    velocity *= enemyFab.speed;
    enemy->AddComponent<Transform>(
            Magnum::Vector2{0.0f, 0.0f},
            Magnum::Vector2{enemyFab.scale, enemyFab.scale},
            velocity,
            0.0f
        );
    enemy->AddComponent<LifeSpan>(15.0f, m_timeline.currentFrameTime());
    enemy->AddSprite<CircleSprite>(m_shader, enemyFab.radius, enemyFab.color, enemyFab.num_segments);
}

void GameEngine::clearEnemies()
{
    const auto& enemies = m_entityManager.getEntityById(Enemy);
    for (auto& e : enemies) e->Destroy();
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
    if (ImGui::CollapsingHeader("Enemy Presets"))
    {
        if (ImGui::Button("Clear Enemies")) clearEnemies();

        if (ImGui::CollapsingHeader("Runner"))
        {
            ImGui::ColorPicker3("Color", m_runnerFab.color.data());
            ImGui::SliderFloat("Movement Speed", &m_runnerFab.speed, 1.0f, 10.0f);
            ImGui::SliderFloat("Scale", &m_runnerFab.scale, 0.01f, 1.0f);
            ImGui::SliderInt("NumSegments", &m_runnerFab.num_segments, 3, 64);
        }

        if (ImGui::CollapsingHeader("Heavy"))
        {
            ImGui::ColorPicker3("Color", m_heavyFab.color.data());
            ImGui::SliderFloat("Movement Speed", &m_heavyFab.speed, 1.0f, 10.0f);
            ImGui::SliderFloat("Scale", &m_heavyFab.scale, 0.01f, 1.0f);
            ImGui::SliderInt("NumSegments", &m_heavyFab.num_segments, 3, 64);
        }
    }

    m_imguiContext.updateApplicationCursor(*this);


    m_imguiContext.drawFrame();
}

void GameEngine::integratePlayerPosition(Transform& t)
{
    const float dt = m_timeline.currentFrameDuration();
    Magnum::Vector2 newPosition {t.position.x(), t.position.y()};

    newPosition.x() += t.velocity.x() * dt;
    newPosition.y() += t.velocity.y() * dt;

    if (newPosition.x() - t.scale.x() / 2 < -windowDimensions().x() / 2 || newPosition.x() + t.scale.x() / 2 > windowDimensions().x() / 2) newPosition.x() = t.position.x();
    if (newPosition.y() - t.scale.y() / 2 < -windowDimensions().y() / 2 || newPosition.y() + t.scale.y() / 2 > windowDimensions().y() / 2) newPosition.y() = t.position.y();

    t.position = newPosition;
}

void GameEngine::integratePosition(Transform& t)
{
    const float dt = m_timeline.currentFrameDuration();
    auto tmp_position = t.position + t.velocity * dt;
    if (tmp_position.x() - t.scale.x() / 2 < -windowDimensions().x() / 2 || tmp_position.x() + t.scale.x() / 2 > windowDimensions().x() / 2) {
        t.velocity = {-t.velocity.x(), t.velocity.y()};
    }
    if (tmp_position.y() - t.scale.y() / 2 < -windowDimensions().y() / 2 || tmp_position.y() + t.scale.y() / 2 > windowDimensions().y() / 2) {
        t.velocity = {t.velocity.x(), -t.velocity.y()};
    }
    t.position += t.velocity * dt;
}
void GameEngine::sMovement()
{

    for (const auto& e : m_entityManager.getEntityById(Enemy))
        integratePosition(e->Get<Transform>());

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
    integratePlayerPosition(t);
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