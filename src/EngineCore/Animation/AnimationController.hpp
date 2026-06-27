#ifndef INCLUDE_ANIMATION_ANIMATIONCONTROLLER_HPP_
#define INCLUDE_ANIMATION_ANIMATIONCONTROLLER_HPP_

#include "Components/Transform.hpp"
#include "Corrade/Containers/StaticArray.h"
#include "Corrade/Utility/DebugAssert.h"
#include "Utilities/Timer.hpp"

#include <cstddef>
template <std::size_t size> class TransformAnimationController {
  static_assert(size != 0, "Animation must have at least one frame");

  Corrade::Containers::StaticArray<size, Transform> m_frames;
  std::size_t m_currentFrame;
  long m_frameTime;
  long m_currentFrameDuration;

  Timer m_timer;

public:
  TransformAnimationController<size>(
      Corrade::Containers::StaticArray<size, Transform> frames, const long fps)
      // @TODO: Remove the hardcoded values
      : m_frames(frames), m_currentFrame(), m_frameTime(), m_timer() {
    CORRADE_ASSERT(fps > 0, "FPS must be greater than zero", );
    m_frameTime = static_cast<long>(1000.0 / fps);
  }
  static constexpr std::size_t N_Frames = size;

  Transform lerp(float progress, std::size_t firstFrame,
                 std::size_t secondFrame) {
    auto cTransform = m_frames[firstFrame];
    auto nTransform = m_frames[secondFrame];

    return {
        (1 - progress) * cTransform.position + progress * nTransform.position,
        (1 - progress) * cTransform.scale + progress * nTransform.scale,
        (1 - progress) * cTransform.rotation + progress * nTransform.rotation,
    };
  }

  void tick() {
    m_currentFrameDuration = m_timer.getElapsed();
    if (m_currentFrameDuration > m_frameTime) {
      m_currentFrame += 1;
      m_currentFrame = (m_currentFrame == N_Frames) ? 0 : m_currentFrame;
      m_timer.reset();
    }
  }

  Transform getCurrentTransform() {
    auto nextFrame = m_currentFrame == N_Frames - 1 ? 0 : m_currentFrame + 1;
    float progress = static_cast<float>(m_currentFrameDuration) /
                     static_cast<float>(m_frameTime);

    // @TODO: Decide if we want easing or not
    // progress = Magnum::Math::pow<3>(progress);
    return lerp(progress, m_currentFrame, nextFrame);
  }
};

#endif // INCLUDE_ANIMATION_ANIMATIONCONTROLLER_HPP_
