#include "Engine.h"
#include "Corrade/Containers/ArrayView.h"
#include "Corrade/Containers/StaticArray.h"
#include "Magnum/GL/AbstractFramebuffer.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/Magnum.h"

// @TODO: Remove the hardcoded values;
GameEngine::GameEngine(const Arguments &arguments)
    : Magnum::Platform::Application(
          arguments,
          Configuration{}.setTitle("Mega Mario").setSize({800, 800})),
      m_renderer(), m_entityManager() {
  m_windowSize_f = getWindowSizeFloat();

  PlayerConfig config{{400, 400}, {150, 150}, {}};
  setupPlayer(config);
}

void GameEngine::drawEvent() {
  namespace GL = Magnum::GL;
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
  updatePlayer();
  drawPlayer();
  swapBuffers();
}

void GameEngine::setupPlayer(PlayerConfig &config) {
  auto &player = m_entityManager.getPlayer();
  auto playerScale = getNormalizedDeviceScale(config.scale);
  player.setScale(playerScale);
  player.setPosition(getNormalizedDeviceCoordinates(config.initialPosition));

  auto animation = Corrade::Containers::StaticArray<4, Transform>{
      Transform({0.0f, 0.0f}, {0.25f, 0.25f}, 0.0f),
      Transform({0.0f, 0.0f}, {0.5f, 0.5f}, 0.0f),
      Transform({0.0f, 0.0f}, {0.75f, 0.75f}, 0.0f),
      Transform({0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f),
  };
  player.setAnimation(animation);
}

Magnum::Vector2 GameEngine::getNormalizedDeviceCoordinates(
    const Magnum::Vector2i coords) const {
  CORRADE_ASSERT(m_windowSize_f > Magnum::Vector2{},
                 "m_windowSize_f has not been initialised.", {});
  auto coords_f = static_cast<Magnum::Vector2>(coords);
  Magnum::Vector2 ndc{
      2 * coords_f.x() / m_windowSize_f.x() - 1.0f,
      2 * coords_f.y() / m_windowSize_f.y() - 1.0f,
  };
  CORRADE_ASSERT(ndc <= Magnum::Vector2{1.0f} && ndc >= Magnum::Vector2{-1.0f},
                 "m_windowSize_f has not been initialised.", {});
  return ndc;
}

Magnum::Vector2 GameEngine::getWindowSizeFloat() const {
  auto size = windowSize();
  Magnum::Vector2 size_f = static_cast<Magnum::Vector2>(size);
  CORRADE_ASSERT(size_f > Magnum::Vector2{},
                 "The window size is not greater than 0.", {});
  return size_f;
}

Magnum::Vector2
GameEngine::getNormalizedDeviceScale(const Magnum::Vector2i scale) const {
  CORRADE_ASSERT(m_windowSize_f > Magnum::Vector2{},
                 "m_windowSize_f has not been initialised.", {});
  auto scale_f = static_cast<Magnum::Vector2>(scale);
  return {scale_f.x() / m_windowSize_f.x(), scale_f.y() / m_windowSize_f.y()};
}

void GameEngine::updatePlayer() {
  auto &player = m_entityManager.getPlayer();
  player.update();
}
void GameEngine::drawPlayer() {
  auto &player = m_entityManager.getPlayer();
  m_renderer.drawEntity(player.getDrawData());
}

void GameEngine::tickEvent() { redraw(); }

MAGNUM_APPLICATION_MAIN(GameEngine);
