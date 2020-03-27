#pragma once

#include "Drawable.h"
#include "Transformable.h"
#include "Texture.h"
#include "IntRect.h"

namespace sb
{
	class Sprite : public Drawable, public Transformable
	{
	public:
		Sprite() 
			: m_texture(NULL)
		{ }

		Sprite(Texture& texture)
			: m_texture(&texture)
		{ }
		
		inline Texture* getTexture() { return m_texture; }

		void setTexture(Texture& texture);

		void setTexture(Texture& texture, const IntRect& textureArea);

		inline Mesh& getMesh() { return SpriteMesh; }
		
		virtual void draw(DrawTarget& window, DrawStates states = DrawStates::getDefault());

	private:
		static Mesh SpriteMesh;

		Texture* m_texture;

		Transform m_textureTransform;
	};
}