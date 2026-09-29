#include "EngineCore/Engine.h"

Magnum::Vector2 GameEngine::getNormalizedDeviceCoordinates(const Magnum::Vector2i coords) const {
    const auto size = static_cast<Magnum::Vector2>(this->windowSize());
    auto coords_f = static_cast<Magnum::Vector2>(coords);
    Magnum::Vector2 ndc{
        2 * coords_f.x() / size.x() - 1.0f,
        2 * coords_f.y() / size.y() - 1.0f,
    };
    return ndc;
}
