#pragma once
#include "Easing.h"
#include <vector>

namespace sb
{

	template <class T>
    class Tween
    {
        typedef T(*easingFunction)(float t, float t0, float t1, const T& from, const T& to);

        struct EasingCommand {
            float t0;
            float t1;
            T from;
            T to;
            easingFunction easing;

            EasingCommand() { }

            EasingCommand(float t0_, float t1_, const T& from_, const T& to_, easingFunction easing_)
                : t0(t0_), t1(t1_), from(from_), to(to_), easing(easing_)
            { }
        };

        std::vector<EasingCommand> _easingCommands;

    protected:
		inline EasingCommand getCurrentEasingCommand(float t) const {
			for (std::size_t i = 0; i < _easingCommands.size(); i++) {
				if (t >= _easingCommands[i].t0 && t < _easingCommands[i].t1)
					return _easingCommands[i];
			}

			SB_ERROR("invalid parameter value");
			return EasingCommand();
		}

        template <easingFunction F>
        inline Tween<T>& addEasing(const T& from, const T& to, float duration) {
            float t0 = _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
            float t1 = t0 + duration;
            _easingCommands.push_back(EasingCommand(t0, t1, from, to, F));
            return *this;
        }

    public:
        Tween() { }

		inline T value(float t) const {
			if (_easingCommands.empty())
				return 0;

			if (t < _easingCommands[0].t0)
				return _easingCommands[0].from;

			if (t >= _easingCommands[_easingCommands.size() - 1].t1)
				return _easingCommands[_easingCommands.size() - 1].to;

			EasingCommand easingCommand = getCurrentEasingCommand(t);
			return easingCommand.easing(t, easingCommand.t0, easingCommand.t1, easingCommand.from, easingCommand.to);
		}

		inline T targetValue() const {
			if (_easingCommands.empty())
				return 0;

			return _easingCommands[_easingCommands.size() - 1].to;
		}

		inline float getDuration() const {
			return _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
		}

		inline std::vector<EasingCommand>& getEasingCommands() { return _easingCommands; }

		inline void stretchDuration(float newDuration) {
			float factor = newDuration / getDuration();
			for (size_t i = 0; i < _easingCommands.size(); i++) {
				_easingCommands[i].t0 *= factor;
				_easingCommands[i].t1 *= factor;
			}
		}

		void scale(T factor) {
			for (size_t i = 0; i < _easingCommands.size(); i++) {
				_easingCommands[i].from *= factor;
				_easingCommands[i].to *= factor;
			}
		}

		Tween None(const T& value = T()) {
			return Tween().linear(value, value, 0);
		}

		Tween& wait(float duration) {
			const T& value = !_easingCommands.empty() ? _easingCommands[_easingCommands.size() - 1].to : 0;
			return wait(value, duration);
		}

        inline Tween& wait(const T& value, float duration) { return addEasing<Easing<T>::linear>(value, value, duration); }

		inline Tween<T>& linear(const T& from, const T& to, float duration) { return addEasing<Easing<T>::linear>(from, to, duration);  }

        inline Tween& sineIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::sineIn>(from, to, duration); }

        inline Tween& sineOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::sineOut>(from, to, duration); }
                     
        inline Tween& sineInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::sineInOut>(from, to, duration); }
                 
        inline Tween& quadIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quadIn>(from, to, duration); }

        inline Tween& quadOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quadOut>(from, to, duration); }

        inline Tween& quadInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quadInOut>(from, to, duration); }
              
        inline Tween& cubicIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::cubicIn>(from, to, duration); }
                
        inline Tween& cubicOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::cubicOut>(from, to, duration); }
                 
        inline Tween& cubicInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::cubicInOut>(from, to, duration); }

        inline Tween& quartIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quartIn>(from, to, duration); }

        inline Tween& quartOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quartOut>(from, to, duration); }

        inline Tween& quartInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quartInOut>(from, to, duration); }
                      
        inline Tween& quintIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quintIn>(from, to, duration); }

        inline Tween& quintOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quintOut>(from, to, duration); }
                     
        inline Tween& quintInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::quintInOut>(from, to, duration); }

		inline Tween& expoIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::expoIn>(from, to, duration); }

		inline Tween& expoOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::expoOut>(from, to, duration); }

		inline Tween& expoInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::expoInOut>(from, to, duration); }

		inline Tween& circIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::circIn>(from, to, duration); }

		inline Tween& circOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::circOut>(from, to, duration); }

		inline Tween& circInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::circInOut>(from, to, duration); }

		inline Tween& backIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::backIn>(from, to, duration); }

		inline Tween& backOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::backOut>(from, to, duration); }

		inline Tween& backInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::backInOut>(from, to, duration); }

		inline Tween& elasticIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::elasticIn>(from, to, duration); }

		inline Tween& elasticOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::elasticOut>(from, to, duration); }

		inline Tween& elasticInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::elasticInOut>(from, to, duration); }

		inline Tween& bounceIn(const T& from, const T& to, float duration) { return addEasing<Easing<T>::bounceIn>(from, to, duration); }

		inline Tween& bounceOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::bounceOut>(from, to, duration); }

		inline Tween& bounceInOut(const T& from, const T& to, float duration) { return addEasing<Easing<T>::bounceInOut>(from, to, duration); }
    };

	typedef Tween<float> Tweenf;
	typedef Tween<Vector2f> Tween2f;
}
