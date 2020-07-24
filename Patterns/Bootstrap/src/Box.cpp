#include "Box.h"
#include "Math.h"
#include <algorithm>

namespace sb 
{ 
	Vector2f Box::random() const
	{
		float x = sb::random(-_size.x / 2, _size.x / 2);
		float y = sb::random(-_size.y / 2, _size.y / 2);
		return Vector2f(x, y);
	}

	float Box::getBoundingRadius() const
	{
		float diameter = std::max(_size.x, _size.y);
		return diameter / 2;
	}

	Shape * Box::clone() const
	{
		return new Box(*this);
	}
}
