#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include <vector>

namespace blocks {

	class Line : public sb::Drawable, public sb::Transformable {
		std::vector<sb::Vector2f> _points;
		float _thickness;
		sb::Color _color;
		std::vector<sb::Vertex> _vertices;
		bool _verticesNeedUpdate;

	protected:
		void addSegment(const sb::Vector2f& start, const sb::Vector2f& end);

		void updateVertices();

	public:
		Line(float thickness = 0.1f, const sb::Color& color = sb::Color(1, 0, 0, 1))
			: _thickness(thickness), _color(color), _verticesNeedUpdate(false)
		{ }

		const std::vector<sb::Vertex>& getVertices();

		inline void addPoint(const sb::Vector2f& point) { addPoint(point.x, point.y); }

		void addPoint(float x, float y);

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
	};
}