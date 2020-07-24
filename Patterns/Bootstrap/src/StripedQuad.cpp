#include "StripedQuad.h"
#include "DrawTarget.h"

namespace blocks {
	void StripedQuad::createMesh(const sb::Vector2f& extent) {
		float stripeWidth = 2 * extent.x / _numStripes;
		static const sb::Color black(0, 0, 0, _alpha);
		static const sb::Color white(1, 1, 1, 0);

		for (size_t i = 0; i < _numStripes; i++) {
			bool isEven = i % 2 == 0;
			const sb::Color& stripeBottomColor = isEven ? black : white;
			const sb::Color& stripeTopColor = isEven ? black : white;
			float stripeLeft = -extent.x + i * stripeWidth;
			float stripeRight = stripeLeft + stripeWidth;
			_mesh[i * 6 + 0] = sb::Vertex(sb::Vector2f(stripeLeft, -extent.y), stripeBottomColor);
			_mesh[i * 6 + 1] = sb::Vertex(sb::Vector2f(stripeLeft, -extent.y), stripeBottomColor);
			_mesh[i * 6 + 2] = sb::Vertex(sb::Vector2f(stripeLeft, +extent.y), stripeTopColor);
			_mesh[i * 6 + 3] = sb::Vertex(sb::Vector2f(stripeRight, -extent.y), stripeBottomColor);
			_mesh[i * 6 + 4] = sb::Vertex(sb::Vector2f(stripeRight, +extent.y), stripeTopColor);
			_mesh[i * 6 + 5] = sb::Vertex(sb::Vector2f(stripeRight, +extent.y), stripeTopColor);
		}
	}

	StripedQuad::StripedQuad(size_t numStripes, float alpha) : _mesh(6 * numStripes, sb::PrimitiveType::TriangleStrip),
		_numStripes(numStripes), _alpha(alpha)
	{
		createMesh(sb::Vector2f(0.5f, 0.5f));
	}

	void StripedQuad::draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
	}
}