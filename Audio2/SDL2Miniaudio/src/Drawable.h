#pragma once
#include "Mesh.h"
#include "DrawStates.h"

namespace sb
{
	class DrawTarget;

	class Drawable
	{

	public:
		virtual ~Drawable() { };

		virtual void draw(DrawTarget& target, DrawStates drawStates) = 0;
	};
}