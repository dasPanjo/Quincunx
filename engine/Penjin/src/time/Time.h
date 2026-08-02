
#pragma once

#include <chrono>

namespace Penjin {
    class Time {
    public:
        static Time& get();

        Time();
        virtual ~Time() = default;

        void reset();
        void tick();

        float deltaTime() const { return deltaTime_;}
        float deltaTimeMs() const { return deltaTime_ * 1000.0f; }
        int64_t totalNanoseconds() const { return totalNanoseconds_; }
        int64_t totalNanosecondsMs() const { return totalNanoseconds_ / 1000000; }
        unsigned int frameCount() const { return frameCount_; }

    private:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        using Nanoseconds = std::chrono::nanoseconds;

        TimePoint startTime_;
        TimePoint lastFrameTime_;

        float deltaTime_{};
        int64_t totalNanoseconds_{};
        unsigned int frameCount_{};
    };
}
