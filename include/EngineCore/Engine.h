#include "Entities/EntityManager.hpp"
#include "Input/InputSystem.h"
#include "Renderer.h"

#include <Magnum/Platform/Sdl2Application.h>
#include "Magnum/Magnum.h"

class GameEngine : public Magnum::Platform::Application {

public:
    virtual ~GameEngine() = default;
    explicit GameEngine(const Arguments& arguments);

private:
    void drawEvent() override;
    void tickEvent() override;
    void keyPressEvent(KeyEvent& event) override;

    Magnum::Vector2 getNormalizedDeviceCoordinates(const Magnum::Vector2i scale) const;
    Magnum::Vector2 getNormalizedDeviceScale(const Magnum::Vector2i scale) const;

    Renderer renderer;
    EntityManager entityManager;
    InputSystem inputSystem;
};
