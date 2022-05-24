#pragma once

#include "Drawable.h"
#include "Transformable.h"
#include "Texture.h"
#include "Rect.h"

namespace sb
{
	class Sprite : public Drawable, public Transformable
	{
	public:
		Sprite();

		Sprite(Texture& texture);

		inline Texture* getTexture() { return m_texture; }

		void setTexture(Texture& texture);

		void setTexture(Texture& texture, const IntRect& textureArea);

		void setColor(const Color& color);

		inline Mesh& getMesh() { return m_mesh; }
		
		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());

	private:
		static Mesh SpriteMesh;

		Mesh m_mesh;

		Texture* m_texture;

		Transform m_textureTransform;
	};
}