#include "DrawStates.h"

namespace sb
{
	DrawStates& DrawStates::getDefault()
	{
		static DrawStates defaultStates;
		return defaultStates;
	}

	const bool operator==(const DrawStates& left, const DrawStates& right)
	{
		return left.shader == right.shader && left.texture == right.texture;
	}
}
