#pragma once

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

		Rect(const Rect& area)
			: left(area.left), bottom(area.bottom), width(area.width), height(area.height)
		{ }

		inline T right() const { return left + width; }
		
		inline T top() const { return bottom + height; }

		inline bool contains(sb::Vector2<T> pos) const {
			return pos.x >= left && pos.x <= right() && pos.y >= bottom && pos.y <= top();
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
