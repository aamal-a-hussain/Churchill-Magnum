#ifndef INCLUDE_UTILITIES_TIMER_HPP_
#define INCLUDE_UTILITIES_TIMER_HPP_

#include <chrono>
class Timer {
    long long start{}, end{};
    std::chrono::time_point<std::chrono::high_resolution_clock> time_start;

public:
    Timer() { reset(); }

    void reset() { this->time_start = std::chrono::high_resolution_clock::now(); }

    long long getStart() {
        using namespace std::chrono;
        this->start = time_point_cast<milliseconds>(this->time_start).time_since_epoch().count();
        return this->start;
    }

    long long getEnd() {
        using namespace std::chrono;
        auto current = high_resolution_clock::now();
        this->end = time_point_cast<milliseconds>(current).time_since_epoch().count();
        return this->end;
    }

    long long getElapsed() { return this->getEnd() - this->getStart(); }
};

#endif // INCLUDE_UTILITIES_TIMER_HPP_
