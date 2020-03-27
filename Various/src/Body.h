#pragma once
#include "Vector2.h"
#include "Transformable.h"

namespace sb
{
	struct Body : public Transformable 
	{
		sb::Vector2f velocity;

		float angularVelocity = 0;
	};
}