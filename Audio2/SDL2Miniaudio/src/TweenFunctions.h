#pragma once

#include "Math.h"

namespace sb
{
    namespace tweenFunctions
    {
        inline float linear(float x)
        {
            return x;
        }

        inline float smoothstep(float x)
        {
            return x * x * (3 - 2 * x);
        }

        inline float bounceOut(float x)
        {
            const float n1 = 7.5625f;
            const float d1 = 2.75f;

            if (x < 1 / d1)
                return n1 * x * x;
            else if (x < 2 / d1) {
                x -= 1.5f / d1;
                return n1 * x * x + 0.75f;
            }
            else if (x < 2.5 / d1) {
                x -= 2.25f / d1;
                return n1 * x * x + 0.9375f;
            }

            x -= 2.625f / d1;
            return n1 * x * x + 0.984375f;
        }

        inline float quadInOut(float x) {
            return x < .5f ? 2 * x * x : 1 - (float)pow(-2 * x + 2, 2) / 2;
        }
    }

    typedef float(*TweenFunction)(float);
}