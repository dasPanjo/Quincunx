
#include "Time.h"

#include <chrono>

Penjin::Time& Penjin::Time::get() {
    static Time instance;
    return instance;
}

Penjin::Time::Time() {
    reset();
}

void Penjin::Time::reset() {
    startTime_ = Clock::now();
    lastFrameTime_ = startTime_;
    deltaTime_ = 0.0f;
    totalNanoseconds_ = 0;
    frameCount_ = 0;
}

void Penjin::Time::tick() {
    TimePoint currentFrameTime = Clock::now();

    auto deltaDuration = std::chrono::duration_cast<Nanoseconds>(currentFrameTime - lastFrameTime_);
    auto totalDuration = std::chrono::duration_cast<Nanoseconds>(currentFrameTime - startTime_);

    totalNanoseconds_ = totalDuration.count();
    lastFrameTime_ = currentFrameTime;
    frameCount_++;

    deltaTime_ = std::chrono::duration<float>(deltaDuration).count();

    if (deltaTime_ > 0.1f) deltaTime_ = 0.1f;
}

