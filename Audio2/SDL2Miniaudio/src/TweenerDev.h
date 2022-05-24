#pragma once

#include "TweenDev.h"
#include "Stopwatch.h"

namespace sb
{
	namespace v1
	{
		template <class T>
		class Tweener {
			v2::Tween<T>* _tween;
			bool _hasOwnTween;
			float _secondsElapsed;

		public:
			Tweener(v2::Tween<T>& tween) : _tween(&tween), _hasOwnTween(false), _secondsElapsed(0)
			{ }

			Tweener(const T& startValue) : _hasOwnTween(true), _secondsElapsed(0) {
				_tween = new v2::Tween<T>(startValue);
			}

			Tweener(const Tweener<T>& other) : _hasOwnTween(other._hasOwnTween), _secondsElapsed(other._secondsElapsed) {
				_tween = new v2::Tween<T>(*other._tween);
			}

			virtual ~Tweener() {
				if (_hasOwnTween)
					delete _tween;
			}

			inline Tweener<T>& to(const T& endValue, float duration, v1::TweenFunction tweenFunction) {
				_tween->to(endValue, duration, tweenFunction);
				return *this;
			}

			inline void update(float ds) { _secondsElapsed += ds; }

			inline T getValue() {
				return _tween->getValue(_secondsElapsed);
			}
		};

		typedef Tweener<float> Tweenerf;
		typedef Tweener<Vector2f> Tweener2f;
	}
}