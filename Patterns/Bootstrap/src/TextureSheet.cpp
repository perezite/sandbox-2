#include "TextureSheet.h"
#include "Logger.h"

namespace sb
{
	TextureSheet::TextureSheet(const sb::Vector2i& size, bool allowOversize)
	{
		if (!allowOversize)
			checkSize(size);

		m_texture.createEmpty(size, sb::Color(0, 0, 0, 0));
	}

	void TextureSheet::loadFromAsset(const std::string& assetPath, const sb::Vector2i& targetLeftBottom, bool flipVertically)
	{
		Image image(assetPath, flipVertically);
		m_texture.loadFromImageIntoSubArea(image, targetLeftBottom);

		m_areas.push_back(IntRect(targetLeftBottom.x, targetLeftBottom.y, image.getWidth(), image.getHeight()));
	}

	void TextureSheet::checkSize(const sb::Vector2i& size)
	{
		static int maxEntent = 1024;

		if (size.x > maxEntent || size.y > maxEntent)
			SB_ERROR("The specified texture sheet size is too large. The maximum allowed width/height is " << maxEntent << " pixels");
	}
}

