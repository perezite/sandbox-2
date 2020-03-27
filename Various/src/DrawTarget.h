#pragma once
#include "Vertex.h"
#include "PrimitiveType.h"
#include "DrawStates.h"
#include "Drawable.h"
#include <vector>

namespace sb
{
	class DrawTarget
	{
	public:
		void draw(Drawable* drawable, const DrawStates& states = DrawStates::getDefault());

		void draw(Drawable& drawable, const DrawStates& states = DrawStates::getDefault());

		virtual void draw(const std::vector<Vertex>& vertices,
			const PrimitiveType& primitiveType, const DrawStates& states = DrawStates::getDefault()) = 0;		
	};
}