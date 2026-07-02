#ifndef INCLUDE_ANIMATION_ANIMATIONCONTROLLER_HPP_
#define INCLUDE_ANIMATION_ANIMATIONCONTROLLER_HPP_

#include "Corrade/Containers/StaticArray.h"
#include "Magnum/Math/Functions.h"
#include "Utilities/Timer.hpp"

#include <cstddef>
namespace Animation {
template <std::size_t size, typename T> class AnimationController {
protected:
  static_assert(size != 0, "Animation must have at least one frame");

  Corrade::Containers::StaticArray<size, T> m_frames;
  std::size_t m_currentFrame;
  long m_frameTime;
  long m_currentFrameDuration;

  Timer m_timer;

public:
  AnimationController(Corrade::Containers::StaticArray<size, T> frames,
                      const long fps)
      : m_frames(frames), m_currentFrame(), m_frameTime(), m_timer() {
    CORRADE_ASSERT(fps > 0, "FPS must be greater than zero", );
    m_frameTime = static_cast<long>(1000.0 / fps);
  }
  static constexpr std::size_t N_Frames = size;

  void tick() {
    m_currentFrameDuration = m_timer.getElapsed();
    if (m_currentFrameDuration > m_frameTime) {
      m_currentFrame += 1;
      m_currentFrame = (m_currentFrame == N_Frames) ? 0 : m_currentFrame;
      m_timer.reset();
      m_currentFrameDuration = m_timer.getElapsed();
    }

    CORRADE_ASSERT(m_currentFrameDuration <= m_frameTime,
                   std::format("{} exceeds frame time {} in tick",
                               m_currentFrameDuration, m_frameTime)
                       .c_str(), );
  }

protected:
  float easing(float progress) { return Magnum::Math::pow<3>(progress); }
};
} // namespace Animation
#endif // INCLUDE_ANIMATION_ANIMATIONCONTROLLER_HPP_
