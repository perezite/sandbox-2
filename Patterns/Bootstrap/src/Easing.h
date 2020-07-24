#pragma once
#include "Math.h"

namespace sb
{
	template <class T>
	class Easing {
		typedef T(*computeFunction)(float t, const T& b, const T& c, float d);

	private:
		static inline T computeLinear(float t, const T& b, const T& c, float d) {
			return c * (t / d) + b;
		}

		static inline T computeSineIn(float t, const T& b, const T& c, float d) {
			return -c * cosf(t / d * (sb::Pi / 2)) + c + b;
		}

		static inline T computeSineOut(float t, const T& b, const T& c, float d) {
			return c * sinf(t / d * (sb::Pi / 2)) + b;
		}

		static inline T computeSineInOut(float t, const T& b, const T& c, float d) {
			return -c / 2.f * (cosf(sb::Pi * t / d) - 1) + b;
		}

		static inline T computeQuadIn(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			return c * t0 * t0 + b;
		}

		static inline T computeQuadOut(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			return -c *(t0)*(t0 - 2) + b;
		}

		static inline T computeQuadInOut(float t, const T& b, const T& c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 1;
			if (t0 < 1) return ((c / 2.f)*(t0*t0)) + b;
			return -c / 2.f * (((t1 - 2)*t1) - 1) + b;
		}

		static inline T computeCubicIn(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			return c*t0*t0*t0 + b;
		}

		static inline T computeCubicOut(float t, const T& b, const T& c, float d) {
			float t0 = t / d - 1;
			return c*(t0*t0*t0 + 1) + b;
		}

		static inline T computeCubicInOut(float t, const T& b, const T& c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 2;
			if (t0 < 1) return c / 2.f * t0*t0*t0 + b;
			return c / 2.f * (t1*t1*t1 + 2) + b;
		}

		static inline T computeQuartIn(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			return c*t0*t0*t0*t0 + b;
		}

		static inline T computeQuartOut(float t, const T& b, const T& c, float d) {
			float t0 = t / d - 1;
			return -c * (t0*t0*t0*t0 - 1) + b;
		}

		static inline T computeQuartInOut(float t, const T& b, const T& c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 2;
			if (t0 < 1) return c / 2.f * t0*t0*t0*t0 + b;
			return -c / 2.f * (t1*t1*t1*t1 - 2) + b;
		}

		static inline T computeQuintIn(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			return c*t0*t0*t0*t0*t0 + b;
		}

		static inline T computeQuintOut(float t, const T& b, const T& c, float d) {
			float t0 = t / d - 1;
			return c*(t0*t0*t0*t0*t0 + 1) + b;
		}

		static inline T computeQuintInOut(float t, const T& b, const T& c, float d) {
			float t0 = t / (d / 2);
			if (t0 < 1) return c / 2.f * t0*t0*t0*t0*t0 + b;
			t0 = t0 - 2;
			return c / 2.f * (t0*t0*t0*t0*t0 + 2) + b;
		}

		static inline T computeExpoIn(float t, const T& b, const T& c, float d) {
			return (t == 0) ? b : c * powf(2.0f, 10 * (t / d - 1)) + b;
		}

		static inline T computeExpoOut(float t, const T& b, const T& c, float d) {
			return (t == d) ? b + c : c * (-powf(2.0f, -10 * t / d) + 1) + b;
		}

		static inline T computeExpoInOut(float t, const T& b, const T& c, float d) {
			if (t == 0) return b;
			if (t == d) return b + c;
			if ((t /= d / 2) < 1) return c / 2.f * powf(2.0f, 10 * (t - 1)) + b;
			return c / 2.f * (-powf(2.0f, -10 * --t) + 2) + b;
		}

		static inline T computeCircIn(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			return -c * (sqrtf(1 - t0*t0) - 1) + b;
		}

		static inline T computeCircOut(float t, const T& b, const T& c, float d) {
			float t0 = t / d - 1;
			return c * sqrtf(1 - t0*t0) + b;
		}

		static inline T computeCircInOut(float t, const T& b, const T& c, float d) {
			float t0 = t / (d / 2);
			float t1 = t0 - 2;
			if (t0 < 1) return -c / 2.f * (sqrtf(1 - t0*t0) - 1) + b;
			return c / 2.f * (sqrtf(1 - t1*t1) + 1) + b;
		}

		static inline T computeBackIn(float t, const T& b, const T& c, float d) {
			float s = 1.70158f;
			float postFix = t /= d;
			return c*(postFix)*t*((s + 1)*t - s) + b;
		}

		static inline T computeBackOut(float t, const T& b, const T& c, float d) {
			float s = 1.70158f;
			float t0 = t / d - 1;
			return c*(t0*t0*((s + 1)*t0 + s) + 1) + b;
		}

		static inline T computeBackInOut(float t, const T& b, const T& c, float d) {
			float s = 1.70158f;
			float s0 = s * 1.525f;
			float t0 = t / (d / 2);
			if (t0 < 1) return c / 2.f * (t0*t0*((s0 + 1)*t0 - s0)) + b;
			float postFix = t0 -= 2;
			return c / 2.f * ((postFix)*t0*((s0 + 1)*t0 + s0) + 2) + b;
		}

		static inline T computeElasticIn(float t, const T& b, const T& c, float d) {
			if (t == 0) return b;  if ((t /= d) == 1) return b + c;
			float p = d*.3f;
			T a = c;
			float s = p / 4;
			T postFix = a*powf(2.0f, 10 * (t -= 1)); 
			return -(postFix * sinf((t*d - s)*(2 * Pi) / p)) + b;
		}

		static inline T computeElasticOut(float t, const T& b, const T& c, float d) {
			if (t == 0) return b;  if ((t /= d) == 1) return b + c;
			float p = d*.3f;
			T a = c;
			float s = p / 4;
			return (a*powf(2.0f, -10 * t) * sinf((t*d - s)*(2 * Pi) / p) + c + b);
		}

		static inline T computeElasticInOut(float t, const T& b, const T& c, float d) {
			if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;
			float p = d*(.3f*1.5f);
			T a = c;
			float s = p / 4;

			if (t < 1) {
				T postFix = a*powf(2.0f, 10 * (t -= 1)); 
				return -.5f*(postFix* sinf((t*d - s)*(2 * Pi) / p)) + b;
			}
			T postFix = a*powf(2.0f, -10 * (t -= 1)); 
			return postFix * sinf((t*d - s)*(2 * Pi) / p)*.5f + c + b;
		}

		static inline T computeBounceIn(float t, const T& b, const T& c, float d) {
			return c - computeBounceOut(d - t, 0, c, d) + b;
		}

		static inline T computeBounceOut(float t, const T& b, const T& c, float d) {
			float t0 = t / d;
			if (t0 < (1 / 2.75f)) {
				return c * (7.5625f * t0 * t0) + b;
			}
			else if (t0 < 2 / 2.75f) {
				float postFix = t0 -= (1.5f / 2.75f);
				return c * (7.5625f * postFix * t0 + .75f) + b;
			}
			else if (t0 < 2.5 / 2.75) {
				float postFix = t0 -= (2.25f / 2.75f);
				return c * (7.5625f * postFix * t0 + .9375f) + b;
			}
			else {
				float postFix = t0 -= (2.625f / 2.75f);
				return c * (7.5625f * postFix * t0 + .984375f) + b;
			}
		}

		static inline T computeBounceInOut(float t, const T& b, const T& c, float d) {
			if (t < d / 2) return computeBounceIn(t * 2, 0, c, d) * .5f + b;
			else return computeBounceOut(t * 2 - d, 0, c, d) * .5f + c*.5f + b;
		}

		template <computeFunction F>
		static inline T compute(float t, float t0, float t1, const T& from, const T& to) {
			float duration = t1 - t0;
			t = clamp(t, t0, t1);
			return F(t - t0, from, to - from, duration);
		}

	public:

		static inline T linear(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeLinear>(t, t0, t1, from, to);
		}

		static inline T sineIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeSineIn>(t, t0, t1, from, to);
		}

		static inline T sineOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeSineOut>(t, t0, t1, from, to);
		}

		static inline T sineInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeSineInOut>(t, t0, t1, from, to);
		}

		static inline T quadIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuadIn>(t, t0, t1, from, to);
		}

		static inline T quadOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuadOut>(t, t0, t1, from, to);
		}

		static inline T quadInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuadInOut>(t, t0, t1, from, to);
		}

		static inline T cubicIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeCubicIn>(t, t0, t1, from, to);
		}

		static inline T cubicOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeCubicOut>(t, t0, t1, from, to);
		}

		static inline T cubicInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeCubicInOut>(t, t0, t1, from, to);
		}

		static inline T quartIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuartIn>(t, t0, t1, from, to);
		}

		static inline T quartOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuartOut>(t, t0, t1, from, to);
		}

		static inline T quartInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuartInOut>(t, t0, t1, from, to);
		}

		static inline T quintIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuintIn>(t, t0, t1, from, to);
		}

		static inline T quintOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuintOut>(t, t0, t1, from, to);
		}

		static inline T quintInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeQuintInOut>(t, t0, t1, from, to);
		}

		static inline T expoIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeExpoIn>(t, t0, t1, from, to);
		}

		static inline T expoOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeExpoOut>(t, t0, t1, from, to);
		}

		static inline T expoInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeExpoInOut>(t, t0, t1, from, to);
		}

		static inline T circIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeCircIn>(t, t0, t1, from, to);
		}

		static inline T circOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeCircOut>(t, t0, t1, from, to);
		}

		static inline T circInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeCircInOut>(t, t0, t1, from, to);
		}

		static inline T backIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeBackIn>(t, t0, t1, from, to);
		}

		static inline T backOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeBackOut>(t, t0, t1, from, to);
		}

		static inline T backInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeBackInOut>(t, t0, t1, from, to);
		}

		static inline T elasticIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeElasticIn>(t, t0, t1, from, to);
		}

		static inline T elasticOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeElasticOut>(t, t0, t1, from, to);
		}

		static inline T elasticInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeElasticInOut>(t, t0, t1, from, to);
		}

		static inline T bounceIn(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeBounceIn>(t, t0, t1, from, to);
		}

		static inline T bounceOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeBounceOut>(t, t0, t1, from, to);
		}

		static inline T bounceInOut(float t, float t0, float t1, const T& from, const T& to) {
			return compute<computeBounceInOut>(t, t0, t1, from, to);
		}
	};
}