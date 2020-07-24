#pragma once
#include "Vector2.h"

namespace sb 
{
	class Shape {
	public:
		virtual ~Shape() {}

		virtual sb::Vector2f random() const = 0;

		virtual float getBoundingRadius() const = 0;

		virtual Shape* clone() const = 0;
	};
}