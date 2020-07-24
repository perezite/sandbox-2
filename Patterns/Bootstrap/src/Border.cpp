#include "Border.h"
#include "DrawTarget.h"

namespace blocks 
{
	Border::Border(float thickness, float height = 1, bool addThicknessToSize = false,
		const sb::Color& innerColor,
		const sb::Color& outerColor)
		: _mesh(10, sb::PrimitiveType::TriangleStrip)
	{
		sb::Vector2f extent(0.5f, height / 2);
		extent = addThicknessToSize ? extent + sb::Vector2f(thickness / 2) : extent;
		_mesh[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
		_mesh[1] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
		_mesh[2] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), outerColor);
		_mesh[3] = sb::Vertex(sb::Vector2f(+extent.x - thickness, -extent.y + thickness), innerColor);
		_mesh[4] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), outerColor);
		_mesh[5] = sb::Vertex(sb::Vector2f(+extent.x - thickness, +extent.y - thickness), innerColor);
		_mesh[6] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), outerColor);
		_mesh[7] = sb::Vertex(sb::Vector2f(-extent.x + thickness, +extent.y - thickness), innerColor);
		_mesh[8] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
		_mesh[9] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
	}

	void Border::draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
	}
}