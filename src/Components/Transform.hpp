#ifndef INCLUDE_COMPONENTS_TRANSFORM_HPP_
#define INCLUDE_COMPONENTS_TRANSFORM_HPP_

#include "Magnum/Magnum.h"
#include "Magnum/Math/Vector2.h"

struct Transform {
  Magnum::Vector2 position;
  Magnum::Vector2 scale;
  float rotation;
};

#endif // INCLUDE_COMPONENTS_TRANSFORM_HPP_
