#include "Grid.h"
#include "DrawTarget.h"

namespace blocks {
	void Grid::computeLine(const sb::Vector2f& start, const sb::Vector2f& end) {
		Line line(_thickness, _color);
		line.addPoint(start);
		line.addPoint(end);
		_vertices.insert(_vertices.end(), line.getVertices().begin(), line.getVertices().end());
	}

	void Grid::computeVerticalLines() {
		sb::Vector2f size = getSize();
		sb::Vector2f halfSize = 0.5f * size;
		float delta = size.x / _gridSize.x;
		for (int i = 0; i <= _gridSize.x; i++) {
			sb::Vector2f start(i * delta - halfSize.x, -halfSize.y);
			sb::Vector2f end(i * delta - halfSize.x, +halfSize.y);
			computeLine(start, end);
		}
	}

	void Grid::computeHorizontalLines() {
		sb::Vector2f size = getSize();
		sb::Vector2f halfSize = 0.5f * size;
		float delta = size.y / _gridSize.y;
		for (int i = 0; i <= _gridSize.y; i++) {
			sb::Vector2f start(-halfSize.x, i * delta - halfSize.y);
			sb::Vector2f end(+halfSize.x, i * delta - halfSize.y);
			computeLine(start, end);
		}
	}

	void Grid::computeLines() {
		_vertices.clear();
		sb::Vector2f delta(1.f / _gridSize.x, 1.f / _gridSize.y);

		computeVerticalLines();
		computeHorizontalLines();
	}

	Grid::Grid(sb::Vector2i gridSize, float thickness, const sb::Color& color)
		: _gridSize(gridSize), _thickness(thickness), _color(color)
	{
		computeLines();
	}

		void Grid::draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		target.draw(_vertices, sb::PrimitiveType::TriangleStrip, states);
	}
}
