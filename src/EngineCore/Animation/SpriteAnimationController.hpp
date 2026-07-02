#ifndef INCLUDE_ANIMATION_SPRITEANIMATIONCONTROLLER_HPP_
#define INCLUDE_ANIMATION_SPRITEANIMATIONCONTROLLER_HPP_
#include "AnimationController.hpp"
#include "Components/Sprite.hpp"
#include "Corrade/Containers/StaticArray.h"

namespace Animation {
template <std::size_t size>
class SpriteAnimationController : public AnimationController<size, Sprite> {
public:
  SpriteAnimationController(
      Corrade::Containers::StaticArray<size, Sprite> frames, const long fps)
      : AnimationController<size, Sprite>(frames, fps) {};

  //@NOTE: Returning by value since Sprite just holds a single enum.
  Sprite getCurrentFrame() { return this->m_frames[this->m_currentFrame]; }
};
} // namespace Animation

#endif // INCLUDE_ANIMATION_SPRITEANIMATIONCONTROLLER_HPP_
