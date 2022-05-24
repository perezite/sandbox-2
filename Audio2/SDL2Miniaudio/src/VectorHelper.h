#pragma once 

#include "Vector2.h"
#include <cmath>

namespace blocks {
    inline sb::Vector2f toVector2f(const sb::Vector2i& v) {
        return sb::Vector2f(float(v.x), float(v.y));
    }

    inline sb::Vector2f toVector2f(int x, int y) { return toVector2f(sb::Vector2i(x, y)); }

    inline sb::Vector2i toVector2i(const sb::Vector2f& v) {
        return sb::Vector2i((int)round(v.x), (int)round(v.y));
    }
}