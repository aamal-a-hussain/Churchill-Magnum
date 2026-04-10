//
// Created by aamal on 10/04/2026.
//
#include "GameEngine.h"
#include <Magnum/GL/Renderer.h>

void GameEngine::sImGui()
{
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::ScissorTest);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
    Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

    m_imguiContext.newFrame();

    if (ImGui::Button(m_paused ? "Play" : "Pause")) m_paused = !m_paused;
    ImGui::SameLine();
    if (ImGui::Button("Exit to Desktop")) exit(EXIT_SUCCESS);

    if (ImGui::BeginTabBar("Gui Tabs"))
    {
        if (ImGui::BeginTabItem("Presets"))
        {
            ImGui::SliderFloat("Window Size", &m_windowProperties.windowSize, 1.0f, 20.0f);
            ImGui::SliderFloat("Movement Speed", &m_playerControls.movementSpeed, 1.0f, 10.0f);
            ImGui::SliderFloat("Warp Speed", &m_playerControls.warpSpeed, 5.0f, 20.0f);

            if (ImGui::CollapsingHeader("UI"))
            {
                ImGui::SliderFloat("Score Size", &m_scoreUIProperties.size, 1E-3f, 1E-2f);
                ImGui::DragFloat("Score X", &m_scoreUIProperties.position.x(), 0.01f, -1.0f, 1.0f);
                ImGui::DragFloat("Score Y", &m_scoreUIProperties.position.y(), 0.01f, -1.0f, 1.0f);

                ImGui::SliderFloat("Health Size", &m_healthUIProperties.size, 1E-3f, 1E-2f);
                ImGui::DragFloat("Health X", &m_healthUIProperties.position.x(), 0.01f, -1.0f, 1.0f);
                ImGui::DragFloat("Health Y", &m_healthUIProperties.position.y(), 0.01f, -1.0f, 1.0f);
            }
            if (ImGui::CollapsingHeader("Enemies"))
            {
                if (ImGui::Button("Clear Enemies")) clearEntity(EntityType::Enemy);
                ImGui::SameLine();
                if (ImGui::Button("Spawn Runner")) spawnEnemy(m_runnerFab);
                ImGui::SameLine();
                if (ImGui::Button("Spawn Heavy")) spawnEnemy(m_heavyFab);

                ImGui::SliderFloat("Collision Capsule Scale", &m_collisionCapsuleScale, 0.1f, 1.0f);
                if (ImGui::CollapsingHeader("Runner Settings"))
                {
                    ImGui::SliderFloat("Runner Speed", &m_runnerFab.speed, 1.0f, 10.0f);
                    ImGui::SliderFloat("Runner Scale", &m_runnerFab.scale, 0.01f, 1.0f);
                    ImGui::SliderInt("Runner Segments", &m_runnerFab.num_segments, 3, 64);
                    ImGui::ColorPicker3("Runner Color", m_runnerFab.color.data());
                }

                if (ImGui::CollapsingHeader("Heavy Settings"))
                {
                    ImGui::SliderFloat("Heavy Speed", &m_heavyFab.speed, 1.0f, 10.0f);
                    ImGui::SliderFloat("Heavy Scale", &m_heavyFab.scale, 0.01f, 1.0f);
                    ImGui::SliderInt("Heavy Segments", &m_heavyFab.num_segments, 3, 64);
                    ImGui::ColorPicker3("Heavy Color", m_heavyFab.color.data());
                }
            }
            if (ImGui::CollapsingHeader("Bullet"))
            {
                if (ImGui::Button("Clear Bullets")) clearEntity(EntityType::Bullet);
                ImGui::SliderFloat("Bullet Speed", &m_bulletFab.speed, 1.0f, 10.0f);
                ImGui::SliderFloat("Bullet Scale", &m_bulletFab.scale, 0.01f, 1.0f);
                ImGui::SliderFloat("Bullet Lifetime", &m_bulletFab.lifetime, 0.1f, 2.0f);
                ImGui::SliderFloat("Shot Interval", &m_bulletFab.shotInterval, 0.1f, .5f);

                ImGui::ColorPicker3("Bullet Color", m_bulletFab.color.data());
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Systems"))
        {
            ImGui::Checkbox("Movement", &m_movementActive);
            ImGui::Checkbox("Rendering", &m_renderingActive);
            ImGui::Checkbox("Spawning", &m_spawnActive);
            ImGui::Checkbox("Shooting", &m_shootActive);
            ImGui::Checkbox("Collisions", &m_collisionActive);
            ImGui::Checkbox("Lifespans", &m_lifespanActive);


            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    m_imguiContext.updateApplicationCursor(*this);


    m_imguiContext.drawFrame();
}