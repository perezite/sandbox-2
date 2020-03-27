#pragma once
#include "Easing.h"
#include <vector>

namespace sb
{
    class Tween
    {
        typedef float(*easingFunction)(float t, float t0, float t1, float from, float to);

        struct EasingCommand {
            float t0;
            float t1;
            float from;
            float to;
            easingFunction easing;

            EasingCommand() { }

            EasingCommand(float t0_, float t1_, float from_, float to_, easingFunction easing_)
                : t0(t0_), t1(t1_), from(from_), to(to_), easing(easing_)
            { }
        };

        std::vector<EasingCommand> _easingCommands;

    protected:
        EasingCommand getCurrentEasingCommand(float t) const;

        template <easingFunction F>
        inline Tween& addEasing(float from, float to, float duration) {
            float t0 = _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
            float t1 = t0 + duration;
            _easingCommands.push_back(EasingCommand(t0, t1, from, to, F));
            return *this;
        }

    public:
        Tween() { }

        float value(float t) const;

        float getDuration() const;

        static Tween None(float value = 0);

        Tween& wait(float duration);

        inline Tween& wait(float value, float duration) { return addEasing<Easing::linear>(value, value, duration); }

        inline Tween& linear(float from, float to, float duration) { return addEasing<Easing::linear>(from, to, duration); }

        inline Tween& sineIn(float from, float to, float duration) { return addEasing<Easing::sineIn>(from, to, duration); }

        inline Tween& sineOut(float from, float to, float duration) { return addEasing<Easing::sineOut>(from, to, duration); }
                      
        inline Tween& sineInOut(float from, float to, float duration) { return addEasing<Easing::sineInOut>(from, to, duration); }
                      
        inline Tween& quadIn(float from, float to, float duration) { return addEasing<Easing::quadIn>(from, to, duration); }
                      
        inline Tween& quadOut(float from, float to, float duration) { return addEasing<Easing::quadOut>(from, to, duration); }
                      
        inline Tween& quadInOut(float from, float to, float duration) { return addEasing<Easing::quadInOut>(from, to, duration); }
                      
        inline Tween& cubicIn(float from, float to, float duration) { return addEasing<Easing::cubicIn>(from, to, duration); }
                      
        inline Tween& cubicOut(float from, float to, float duration) { return addEasing<Easing::cubicOut>(from, to, duration); }
                      
        inline Tween& cubicInOut(float from, float to, float duration) { return addEasing<Easing::cubicInOut>(from, to, duration); }

        inline Tween& quartIn(float from, float to, float duration) { return addEasing<Easing::quartIn>(from, to, duration); }

        inline Tween& quartOut(float from, float to, float duration) { return addEasing<Easing::quartOut>(from, to, duration); }

        inline Tween& quartInOut(float from, float to, float duration) { return addEasing<Easing::quartInOut>(from, to, duration); }
                      
        inline Tween& quintIn(float from, float to, float duration) { return addEasing<Easing::quintIn>(from, to, duration); }

        inline Tween& quintOut(float from, float to, float duration) { return addEasing<Easing::quintOut>(from, to, duration); }
                      
        inline Tween& quintInOut(float from, float to, float duration) { return addEasing<Easing::quintInOut>(from, to, duration); }

		inline Tween& expoIn(float from, float to, float duration) { return addEasing<Easing::expoIn>(from, to, duration); }

		inline Tween& expoOut(float from, float to, float duration) { return addEasing<Easing::expoOut>(from, to, duration); }

		inline Tween& expoInOut(float from, float to, float duration) { return addEasing<Easing::expoInOut>(from, to, duration); }

		inline Tween& circIn(float from, float to, float duration) { return addEasing<Easing::circIn>(from, to, duration); }

		inline Tween& circOut(float from, float to, float duration) { return addEasing<Easing::circOut>(from, to, duration); }

		inline Tween& circInOut(float from, float to, float duration) { return addEasing<Easing::circInOut>(from, to, duration); }

		inline Tween& backIn(float from, float to, float duration) { return addEasing<Easing::backIn>(from, to, duration); }

		inline Tween& backOut(float from, float to, float duration) { return addEasing<Easing::backOut>(from, to, duration); }

		inline Tween& backInOut(float from, float to, float duration) { return addEasing<Easing::backInOut>(from, to, duration); }

		inline Tween& elasticIn(float from, float to, float duration) { return addEasing<Easing::elasticIn>(from, to, duration); }

		inline Tween& elasticOut(float from, float to, float duration) { return addEasing<Easing::elasticOut>(from, to, duration); }

		inline Tween& elasticInOut(float from, float to, float duration) { return addEasing<Easing::elasticInOut>(from, to, duration); }

		inline Tween& bounceIn(float from, float to, float duration) { return addEasing<Easing::bounceIn>(from, to, duration); }

        inline Tween& bounceOut(float from, float to, float duration) { return addEasing<Easing::bounceOut>(from, to, duration); }

		inline Tween& bounceInOut(float from, float to, float duration) { return addEasing<Easing::bounceInOut>(from, to, duration); }
    };
}
