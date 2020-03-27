#include "Disk.h"

namespace sb 
{ 
	Vector2f Disk::random() const
	{
		float r = sqrtf(sb::random(_innerRadius * _innerRadius, _outerRadius * _outerRadius));
		float angle = sb::random(_startRadian, _endRadian);
		auto result = Vector2f(r * cosf(angle), r * sinf(angle));
		return result;	
	}

	Shape * Disk::clone() const
	{
		return new Disk(*this);
	}
}
