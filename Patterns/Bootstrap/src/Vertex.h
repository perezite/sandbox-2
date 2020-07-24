#pragma once
#include "Vector2.h"
#include "Color.h"

namespace sb
{
	struct Vertex
	{
		Vertex() { }

		Vertex(const Vector2f& position_, const Color& color_, const Vector2f& texCoords_ = Vector2f(0, 0))
			: position(position_), color(color_), texCoords(texCoords_)
		{ }

		Vector2f position;

		Color color;

		Vector2f texCoords;
	};
}