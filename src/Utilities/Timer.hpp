#ifndef INCLUDE_UTILITIES_TIMER_HPP_
#define INCLUDE_UTILITIES_TIMER_HPP_

#include <chrono>
class Timer {
  long long m_start{}, m_end{};
  std::chrono::time_point<std::chrono::high_resolution_clock> m_time_start;

public:
  Timer() { reset(); }
  void reset() { m_time_start = std::chrono::high_resolution_clock::now(); }
  long long getElapsed() {
    using namespace std::chrono;
    auto current = high_resolution_clock::now();
    m_start =
        time_point_cast<milliseconds>(m_time_start).time_since_epoch().count();
    m_end = time_point_cast<milliseconds>(current).time_since_epoch().count();
    return m_end - m_start;
  }
};

#endif // INCLUDE_UTILITIES_TIMER_HPP_
