#pragma once
#include "Drawable.h"
#include "Transformable.h"

namespace blocks {
	class Border : public sb::Drawable, public sb::Transformable {
	private:
		sb::Mesh _mesh;

	public:
		Border(float thickness, float height, bool addThicknessToSize,
			const sb::Color& innerColor = sb::Color::createFromRGB(236, 140, 105, 100),
			const sb::Color& outerColor = sb::Color::createFromRGB(237, 110, 160, 100));

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states);
	};
}