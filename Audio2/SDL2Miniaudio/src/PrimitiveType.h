#pragma once 

#include "GL.h"

namespace sb
{
	enum class PrimitiveType
	{
		Points = GL_POINTS,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP
	};
}