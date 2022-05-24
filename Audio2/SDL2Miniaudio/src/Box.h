#pragma once
#include "Shape.h"
#include "Math.h"

namespace sb
{
	class Box : public Shape {
		sb::Vector2f _size;

	public:
		Box(sb::Vector2f size) : _size(size)
		{ }

		Box(float w, float h) : _size(sb::Vector2f(w, h))
		{ }

		virtual Vector2f random() const;

		virtual float getBoundingRadius() const;

		virtual Shape* clone() const;
	};
}