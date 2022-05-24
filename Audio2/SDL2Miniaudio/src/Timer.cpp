#include "Timer.h"

namespace blocks {
    bool Timer::hasTicks()
    {
        _timerSeconds += _stopwatch.getElapsedSeconds();
        _stopwatch.reset();
        
        if (_timerSeconds > _intervalSeconds) {
            _timerSeconds -= _intervalSeconds;
            return true;
        }

        return false;
    }
}
