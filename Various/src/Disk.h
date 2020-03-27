#pragma once
#include "Shape.h"
#include "Math.h"

namespace sb
{
	class Disk : public Shape {
		float _innerRadius;
		float _outerRadius;
		float _startRadian;
		float _endRadian;

	public:
		Disk(float outerRadius)
			: _innerRadius(0), _outerRadius(outerRadius), _startRadian(0), _endRadian(2 * Pi)
		{ }

		Disk(float outerRadius, float startRadian, float endRadian)
			: _innerRadius(0), _outerRadius(outerRadius), _startRadian(startRadian), _endRadian(endRadian)
		{ }

		Disk(float innerRadius, float outerRadius, float startRadian, float endRadian)
			: _innerRadius(innerRadius), _outerRadius(outerRadius), _startRadian(startRadian), _endRadian(endRadian)
		{ }

		virtual Vector2f random() const;

		virtual inline float getBoundingRadius() const { return _outerRadius; }

		virtual Shape* clone() const;
	};
}