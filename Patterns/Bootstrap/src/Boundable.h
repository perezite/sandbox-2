#pragma once
#include "Rect.h"

namespace blocks {
	class Boundable {
	public:
		virtual sb::FloatRect getBounds() = 0;
	};
}