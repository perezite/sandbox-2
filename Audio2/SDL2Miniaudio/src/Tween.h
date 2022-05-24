#pragma once

#include "TweenFunctions.h"
#include "Vector2.h"
#include "Memory.h"
#include "Math.h"

namespace sb
{
        namespace my {
        template <class T> struct Transition {
            T endValue;
            float duration;
            TweenFunction tweenFunction;
            Transition(const T& targetValue_, float duration_, TweenFunction tweenFunction_) :
                endValue(targetValue_), duration(duration_), tweenFunction(tweenFunction_) { }
        };
    }

    template <class T> class Tween
    {
        T _startValue;
        std::vector<my::Transition<T>> _transitions;

    public:
        Tween()
        { }

        Tween(const T& startValue) : _startValue(startValue)
        { }

        Tween<T>& to(const T& endValue, float duration, TweenFunction tweenFunction) {
            my::Transition<T> transition(endValue, duration, tweenFunction);
            _transitions.push_back(transition);
            return *this;
        }

        inline T getValue(float secondsElapsed) {
            float transitionStart = 0; float transitionEnd;

            for (size_t i = 0; i < _transitions.size(); i++) {
                transitionEnd = transitionStart + _transitions[i].duration;
                bool isInTransition = secondsElapsed >= transitionStart && secondsElapsed < transitionEnd;
                if (isInTransition) {
                    float secondsInTransition = secondsElapsed - transitionStart;
                    return getValueInTransition(i, secondsInTransition);
                }

                transitionStart = transitionEnd;
            }

            return last(_transitions)->endValue;
        }

    protected:
        inline T getValueInTransition(size_t transitionIndex, float secondsElapsedInTransition) {
            my::Transition<T>& transition = _transitions[transitionIndex];
            T& startValue = transitionIndex == 0 ? _startValue : _transitions[transitionIndex - 1].endValue;
            float percentage = getPercentage(transition, secondsElapsedInTransition);
            float progress = transition.tweenFunction(percentage);
            return lerp(progress, startValue, transition.endValue);
        }

        inline float getPercentage(my::Transition<T>& transition, float secondsElapsedInTransition) {
            float percentage = secondsElapsedInTransition / transition.duration;
            return clamp(percentage, 0, 1);
        }
    };

    typedef Tween<float> Tweenf;
    typedef Tween<Vector2f> Tween2f;
}