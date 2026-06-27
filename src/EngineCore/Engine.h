#include "Entities/EntityManager.hpp"
#include "Renderer.hpp"

#include "Magnum/Magnum.h"
#include <Magnum/Platform/Sdl2Application.h>

class GameEngine : public Magnum::Platform::Application {

public:
  virtual ~GameEngine() = default;
  explicit GameEngine(const Arguments &arguments);

  static constexpr size_t MAX_NUM_SHAPES = 8;

  struct PlayerConfig {
    Magnum::Vector2i initialPosition;
    Magnum::Vector2i scale;
    float initialRotation;
  };

private:
  void drawEvent() override;
  void tickEvent() override;

  void drawPlayer();
  void updatePlayer();
  void setupPlayer(PlayerConfig &config);

  Magnum::Vector2 getWindowSizeFloat() const;
  Magnum::Vector2
  getNormalizedDeviceCoordinates(const Magnum::Vector2i scale) const;
  Magnum::Vector2 getNormalizedDeviceScale(const Magnum::Vector2i scale) const;

  Magnum::Vector2 m_windowSize_f;
  Renderer m_renderer;
  EntityManager m_entityManager;
};
