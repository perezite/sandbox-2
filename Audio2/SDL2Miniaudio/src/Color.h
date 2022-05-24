#pragma once

namespace sb
{
	struct Color
	{
		Color(float r_ = 1, float g_ = 1, float b_ = 1, float a_ = 1)
			: r(r_), g(g_), b(b_), a(a_)
		{ }

		static sb::Color createFromRGB(int r, int g = 255, int b = 255, int a = 255) {
			return sb::Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
		}

		float r, g, b, a;
	};
}
