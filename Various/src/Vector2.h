#pragma once
#include "Logger.h"
#include <math.h>

namespace sb
{
	template <class T>
	struct Vector2
	{
		inline Vector2()
			: x(T()), y(T())
		{ }

		inline Vector2(T s_)
			: x(s_), y(s_)
		{ }
		
		inline Vector2<T>(T x_, T y_)
			: x(x_), y(y_)
		{ }

		inline Vector2<T> operator-() const
		{
			return Vector2<T>(-this->x, -this->y);
		}

		inline float getLength() const {
			return sqrt(x * x + y * y);
		}

		inline Vector2<float> normalized() const {
			float inverseLength = 1 / getLength();
			return Vector2<float>(inverseLength * this->x, inverseLength * this->y);
		}

		inline Vector2<float> rotated(float radians) const {
			float c = cosf(radians);
			float s = sinf(radians);
			return Vector2<float>(c * x - s * y, s * x + c * y);
		}

		T x, y;
	};

	template <class T>
	inline Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right)
	{
		return Vector2<T>(left.x + right.x, left.y + right.y);
	}

	template <class T>
	inline const Vector2<T>& operator+=(Vector2<T>& left, const Vector2<T>& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	template <class T>
	inline Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right)
	{
		return Vector2<T>(left.x - right.x, left.y - right.y);
	}

	template <class T>
	inline const Vector2<T>& operator-=(Vector2<T>& left, const Vector2<T>& right) {
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}

	template <class T>
	inline Vector2<T> operator*(T scalar, const Vector2<T>& vector)
	{
		return Vector2<T>(scalar * vector.x, scalar * vector.y);
	}

	typedef Vector2<float>	Vector2f;
	typedef Vector2<int>	Vector2i;
}
