#pragma once

#include <math.h>
#include "Vector2.h"
#include "Rect.h"
#define _USE_MATH_DEFINES

namespace sb
{
	const float Pi = (float)M_PI;

	const float ToRadians = 0.01745329251f;
	const float ToDegrees = 57.295779513f;
	
	inline float random() {
		return float(rand()) / float(RAND_MAX);
	}

	template <class T>
	inline T lerp(float t, const T& play, const T& end) {
		return (1 - t) * play + t * end;
	}

	inline float random(float min, float max) {
		return lerp(random(), min, max);
	}

	inline float random(float max) {
		return random(0, max);
	}

	inline Vector2f random2D(float min, float max) {
		return sb::Vector2f(random(min, max), random(min, max));
	}

	inline Vector2f randomOnCircle(float radius) {
		float angle = random(0, 2 * Pi);
		return sb::Vector2f(radius * cosf(angle), radius * sinf(angle));
	}

	inline float angle(const sb::Vector2f& from, const sb::Vector2f& to) {
		return atan2(to.y, to.x) - atan2(from.y, from.x);
	}

	inline int nextPowerOfTwo(int number) {
		int power = 1;
		while (power < number)
			power *= 2;

		return power;
	}

	inline float oscillate(float t, float length)
	{
		double intpart;
		float remainder = (float)modf(t / length, &intpart);
		bool even = (int)intpart % 2 == 0;

		return even ? remainder * length : (1 - remainder) * length;
	}

	inline float clamp(float t, float min, float max) {
		return t > max ? max : t < min ? min : t;
	}

	template <class T>
	Rect<T> computeBounds(const std::vector<Vector2<T>>& v) {
		Vector2<T> min = Vector2<T>(v[0].x, v[0].y), max = min;

		for (size_t i = 0; i < v.size(); i++) {
			const Vector2<T>& val = v[i];
			if (val.x < min.x)
				min.x = val.x;
			if (val.x > max.x)
				max.x = val.x;
			if (val.y < min.y)
				min.y = val.y;
			if (val.y > max.y)
				max.y = val.y;
		}

		return Rect<T>(min, max);
	}
}
