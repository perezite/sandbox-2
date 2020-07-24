#include "Texture.h"
#include "Asset.h"
#include "Math.h"
#include <algorithm>

namespace sb
{
	Texture::Texture(const std::string filePath, bool flipVertically)
		: Texture()
	{
		loadFromAsset(filePath, flipVertically);
	}

	Texture::~Texture() {
		GL_CHECK(glDeleteTextures(1, &m_handle));
	}

	void Texture::computeTransform(const IntRect& area, Transform& result) const
	{
		float sx = float(area.width - 1) / float(m_internalSize.x);
		float sy = float(area.height - 1) / (float)(m_internalSize.y);
		float pixelWidth = 1.0f / m_internalSize.x;
		float pixelHeight = 1.0f / m_internalSize.y;
		float tx = (area.left + 0.5f) * pixelWidth;
		float ty = (area.bottom + 0.5f) * pixelHeight;

		result = Transform(sb::Vector2f(tx, ty), sb::Vector2f(sx, sy), 0);
	}

	void Texture::loadFromAsset(const std::string& assetPath, bool flipVertically)
	{
		SB_ERROR_IF(m_handle != 0, "The texture has already been initialized");

		Image image(assetPath, flipVertically);
		updateSize(image.getSize());

		createEmptyTexture(m_internalSize, sb::Color(0, 0, 0, 0));
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), 
			image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels()));

	}

	void Texture::createEmpty(const sb::Vector2i& size, const Color& color) {
		SB_ERROR_IF(m_handle != 0, "The texture has already been initialized");

		updateSize(size);
		createEmptyTexture(m_internalSize, color);
	}

	void Texture::loadFromImageIntoSubArea(const Image& image, const Vector2i& bottomLeft)
	{
		SB_ERROR_IF(m_handle == 0,
			"This function can only be called on an already initialized texture");

		bool outsideVisibleArea = bottomLeft.x + image.getWidth() > m_size.x ||
			bottomLeft.y + image.getHeight() > m_size.y;
		SB_ERROR_IF(outsideVisibleArea,
			"The existing texture is too small for the loaded image at the given position");

		bind();
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, bottomLeft.x, bottomLeft.y, image.getWidth(),
			image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels()));
	}

	void Texture::loadFromAssetIntoSubArea(const std::string & assetPath, const sb::Vector2i& bottomLeft, bool flipVertically)
	{
		Image image(assetPath, flipVertically);
		loadFromImageIntoSubArea(image, bottomLeft);
	}

	void Texture::enableMipmap(bool enable) {
		if (!isMipmapEnabled() && enable)
			activateMipmap();
		else if (isMipmapEnabled() && !enable)
			deactivateMipmap();
	}

	void Texture::bind() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
	}

	void Texture::updateSize(const sb::Vector2i& size) {
		m_size = size;
		m_internalSize = sb::Vector2i(nextPowerOfTwo(size.x), nextPowerOfTwo(size.y));
	}

	void Texture::createEmptyTexture(const sb::Vector2i& size, const Color& color)
	{
		std::vector<GLubyte> pixels(4 * size.x * size.y);
		for (std::size_t i = 0; i < pixels.size(); i += 4) {
			pixels[i + 0] = int(color.r * 255);
			pixels[i + 1] = int(color.g * 255);
			pixels[i + 2] = int(color.b * 255);
			pixels[i + 3] = int(color.a * 255);
		}

		createGlTexture(size, pixels.data());
	}

	void Texture::createGlTexture(const sb::Vector2i& size, void* pixels)
	{
		GL_CHECK(glGenTextures(1, &m_handle));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
	}

	void Texture::activateMipmap()
	{
		if (!m_isMipmapGenerated) {
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			m_isMipmapGenerated = true;
		}

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		m_isMipmapEnabled = true;
	}

	void Texture::deactivateMipmap()
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		m_isMipmapEnabled = false;
	}
}