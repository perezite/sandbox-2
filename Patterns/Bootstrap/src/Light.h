#pragma once
#include "Vector2.h"

namespace blocks {
	class Light {
		sb::Vector2f _direction;

	public:
		Light(const sb::Vector2f& direction = sb::Vector2f(0, -1)) : _direction(direction)
		{ }

		inline void setDirection(const sb::Vector2f& direction) { _direction = direction; }

		inline const sb::Vector2f& getDirection() const { return _direction; }
	};
}