#pragma once
#include "Texture.h"
#include <vector>

namespace sb
{
	class TextureSheet 
	{
	public:
		TextureSheet(const sb::Vector2i& size, bool allowOversize = true);

		inline const Texture& getTexture() const { return m_texture; }

		inline Texture& getTexture() { return m_texture; }

		inline const sb::IntRect& getArea(std::size_t index) const { return m_areas[index]; };

		void loadFromAsset(const std::string& assetPath, const sb::Vector2i& targetLeftBottom, bool flipVertically = true);

	protected:
		void checkSize(const sb::Vector2i& size);

	private:
		Texture m_texture;

		std::vector<sb::IntRect> m_areas;
	};
}