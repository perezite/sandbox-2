#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include "Camera.h"

namespace blocks {
	class Backdrop : public sb::Drawable, public sb::Transformable {
		sb::Mesh _mesh;

	public:
		Backdrop() : _mesh(4, sb::PrimitiveType::TriangleStrip) {
		}

		void update(sb::Camera& camera);

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states);
	};
}