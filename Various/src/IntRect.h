#pragma once

namespace sb 
{
	struct IntRect 
	{
		IntRect()
			: left(0), bottom(0), width(0), height(0)
		{ }

		IntRect(int left_, int bottom_, int width_, int height_)
			: left(left_), bottom(bottom_), width(width_), height(height_)
		{ }

		IntRect(const IntRect& area)
			: left(area.left), bottom(area.bottom), width(area.width), height(area.height)
		{ }

		int left; int bottom;
		int width; int height;
	};

	inline bool operator==(const IntRect& first, const IntRect&& second) {
		return  first.left == second.left && first.bottom == second.bottom &&
			first.width == second.width && first.height == second.height;
	}
}
