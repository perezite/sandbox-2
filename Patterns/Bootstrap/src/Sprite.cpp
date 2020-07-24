#include "Sprite.h"
#include "DrawTarget.h"

namespace sb
{
	Mesh Sprite::SpriteMesh({
		Vertex(Vector2f(-0.5f, -0.5f), Color(1, 1, 1, 1), Vector2f(0, 0)),
		Vertex(Vector2f( 0.5f, -0.5f), Color(1, 1, 1, 1), Vector2f(1, 0)),
		Vertex(Vector2f(-0.5f,  0.5f), Color(1, 1, 1, 1), Vector2f(0, 1)),
		Vertex(Vector2f( 0.5f,  0.5f), Color(1, 1, 1, 1), Vector2f(1, 1))
	}, PrimitiveType::TriangleStrip);

	Sprite::Sprite()
		: m_mesh(Sprite::SpriteMesh)
	{ }

	Sprite::Sprite(Texture& texture)
		: m_mesh(Sprite::SpriteMesh), m_texture(&texture)
	{ }

	void initMesh() {
	}

	void Sprite::setTexture(Texture& texture)
	{
		const Vector2i size = texture.getSize();
		setTexture(texture, IntRect(0, 0, size.x, size.y));
	}

	void Sprite::setTexture(Texture& texture, const IntRect& textureArea)
	{
		m_texture = &texture; 
		texture.computeTransform(textureArea, m_textureTransform);
	}

	void Sprite::setColor(const Color& color) {
		for (size_t i = 0; i < m_mesh.getVertexCount(); i++)
			m_mesh[i].color = color;
	}


	void Sprite::draw(DrawTarget& target, DrawStates states)
	{
		states.transform *= getTransform();
		states.texture = m_texture;
		states.textureTransform = m_textureTransform;
		target.draw(m_mesh.getVertices(), m_mesh.getPrimitiveType(), states);
	}
}
