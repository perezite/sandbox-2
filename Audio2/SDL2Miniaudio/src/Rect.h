#pragma once

#include "Vector2.h"

namespace sb 
{
	template <class T>
	struct Rect 
	{
		Rect()
			: left(0), bottom(0), width(0), height(0)
		{ }

		Rect(T left_, T bottom_, T width_, T height_)
			: left(left_), bottom(bottom_), width(width_), height(height_)
		{ }

		Rect(const Vector2<T>& bottomLeft, const Vector2<T>& topRight)
			: left(bottomLeft.x), bottom(bottomLeft.y), width(topRight.x - bottomLeft.x + 1), 
			  height(topRight.y - bottomLeft.y + 1)
		{ }

		Rect(const Rect& other)
			: left(other.left), bottom(other.bottom), width(other.width), height(other.height)
		{ }

		inline T right() const { return left + width - 1; }
		
		inline T top() const { return bottom + height -1; }

		inline Vector2<T> bottomLeft() { return Vector2<T>(left, bottom); }

		inline Vector2<T> topRight() { return Vector2<T>(right(), top()); }

		inline bool contains(const sb::Vector2<T>& pos) const {
			return pos.x >= left && pos.x <= right() && pos.y >= bottom && pos.y <= top();
		}

		inline bool contains(const Rect<T>& other) const {
			return other.left >= left && other.right() <= right() && 
				other.bottom >= bottom && other.top() <= top();
		}

		T left; T bottom;
		T width; T height;
	};

	template <class T>
	inline bool operator==(const Rect<T>& first, const Rect<T>& second) {
		return  first.left == second.left && first.bottom == second.bottom &&
			first.width == second.width && first.height == second.height;
	}

	typedef Rect<int> IntRect;
	typedef Rect<float> FloatRect;
}
