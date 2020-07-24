#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include "Line.h"
#include <vector>

namespace blocks {
	class Grid : public sb::Drawable, public sb::Transformable {
		sb::Vector2i _gridSize;
		float _thickness;
		sb::Color _color;
		std::vector<sb::Vertex> _vertices;

	protected:
		void computeLine(const sb::Vector2f& start, const sb::Vector2f& end);

		inline sb::Vector2f getSize() {
			return sb::Vector2f(1, _gridSize.y / float(_gridSize.x));
		}

		void computeVerticalLines();

		void computeHorizontalLines();

		void computeLines();

	public:
		Grid(sb::Vector2i gridSize, float thickness = 0.1f, const sb::Color& color = sb::Color(1, 0, 0, 1));

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
	};
}