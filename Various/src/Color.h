#pragma once

namespace sb
{
	struct Color
	{
		Color(float r_ = 1, float g_ = 1, float b_ = 1, float a_ = 1)
			: r(r_), g(g_), b(b_), a(a_)
		{ }

		float r, g, b, a;
	};
}
