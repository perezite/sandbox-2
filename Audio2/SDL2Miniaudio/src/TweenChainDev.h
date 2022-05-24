#pragma once

#include "TweenDev.h"
#include "Memory.h"
#include <vector>

namespace sb 
{	
	namespace v1 {
		template <class T> class TweenChain
		{
			T _startValue;

			std::vector<v1::Tween<T>> _tweens;

		protected:
			const T& getPreviousTargetValue()
			{
				if (!_tweens.empty())
					return last(_tweens)->getTargetValue();

				return _startValue;
			}

		public:
			TweenChain(const T& startValue)
				: _startValue(startValue)
			{ }

			inline TweenChain& to(const T& endValue, float duration, TweenFunction tweenFunction)
			{
				v1::Tween<T> tween(getPreviousTargetValue(), endValue, duration, tweenFunction);
				_tweens.push_back(tween);
				return *this;
			}
		};

		typedef TweenChain<float> TweenChainf;
		typedef TweenChain<Vector2f> TweenChain2f;
	}
}