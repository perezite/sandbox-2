#include "Line.h"
#include "DrawTarget.h"

namespace blocks {
	void Line::addSegment(const sb::Vector2f& start, const sb::Vector2f& end) {
		float half = _thickness / 2;
		sb::Vector2f dist = end - start;
		sb::Vector2f perp = sb::Vector2f(-dist.y, dist.x).normalized();
		std::vector<sb::Vertex> segment(6);

		segment[0] = sb::Vertex(start - half * perp, _color);
		segment[1] = segment[0];
		segment[2] = sb::Vertex(start + half * perp, _color);
		segment[3] = sb::Vertex(end - half * perp, _color);
		segment[4] = sb::Vertex(end + half * perp, _color);
		segment[5] = segment[4];

		_vertices.insert(_vertices.end(), segment.begin(), segment.end());
	}

	void Line::updateVertices() {
		_vertices.clear();
		for (size_t i = 1; i < _points.size(); i++)
			addSegment(_points[i - 1], _points[i]);

		_verticesNeedUpdate = false;
	}

	const std::vector<sb::Vertex>& Line::getVertices() {
		if (_verticesNeedUpdate)
			updateVertices();
		return _vertices;
	}

	void Line::addPoint(float x, float y) {
		_points.push_back(sb::Vector2f(x, y));
		_verticesNeedUpdate = true;
	}

	void Line::draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		if (_verticesNeedUpdate)
			updateVertices();

		target.draw(_vertices, sb::PrimitiveType::TriangleStrip, states);
	}
}