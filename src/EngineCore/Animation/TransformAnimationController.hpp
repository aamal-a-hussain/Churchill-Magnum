#ifndef INCLUDE_ANIMATION_TRANSFORMANIMATIONCONTROLLER_HPP_
#define INCLUDE_ANIMATION_TRANSFORMANIMATIONCONTROLLER_HPP_

#include "AnimationController.hpp"
#include "Components/Transform.hpp"
#include "Corrade/Utility/Math.h"
namespace Animation {

template <std::size_t size>
class TransformAnimationController
    : public AnimationController<size, Transform> {

public:
  // CONSTRUCTORS
  TransformAnimationController(
      Corrade::Containers::StaticArray<size, Transform> frames, const long fps)
      : AnimationController<size, Transform>(frames, fps) {}
  // METHODS
public:
  Transform lerp(float progress, std::size_t firstFrame,
                 std::size_t secondFrame) {
    auto cTransform = this->m_frames[firstFrame];
    auto nTransform = this->m_frames[secondFrame];

    return {
        (1 - progress) * cTransform.position + progress * nTransform.position,
        (1 - progress) * cTransform.scale + progress * nTransform.scale,
        (1 - progress) * cTransform.rotation + progress * nTransform.rotation,
    };
  }

  Transform getCurrentFrame() {
    auto nextFrame = this->m_currentFrame == this->N_Frames - 1
                         ? 0
                         : this->m_currentFrame + 1;
    CORRADE_ASSERT(
        this->m_currentFrameDuration <= this->m_frameTime,
        std::format("{} exceeds frame time {} in getCurrentTransform",
                    this->m_currentFrameDuration, this->m_frameTime)
            .c_str(),
        {});
    float progress = static_cast<float>(this->m_currentFrameDuration) /
                     static_cast<float>(this->m_frameTime);
    progress = Corrade::Utility::min(progress, 1.0f);
    progress = Corrade::Utility::max(progress, 0.0f);

    CORRADE_ASSERT(progress >= 0.0f && progress <= 1.0f,
                   std::format("Progress {} not in [0, 1]", progress).c_str(),
                   {});

    return lerp(progress, this->m_currentFrame, nextFrame);
  }
};
}; // namespace Animation

#endif // INCLUDE_ANIMATION_TRANSFORMANIMATIONCONTROLLER_HPP_
