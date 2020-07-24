#pragma once
#include "SDL.h"
#include "GL.h"
#include "Color.h"
#include "Vector2.h"
#include "Transform.h"
#include "Rect.h"
#include "Image.h"
#include <string>

namespace sb
{
	class Texture 
	{
	public:
		Texture()
		: m_handle(0), m_isMipmapEnabled(false), m_isMipmapGenerated(false)
		{ }

		~Texture();

		Texture(const std::string filePath, bool flipVertically = true);
		
		inline const Vector2i& getSize() { return m_size; }

		inline const bool isMipmapEnabled() const { return m_isMipmapEnabled; }
		
		void computeTransform(const IntRect& area, Transform& result) const;

		void loadFromAsset(const std::string& assetPath, bool flipVertically = true);
		
		void createEmpty(const Vector2i& size, const Color& color = Color(0, 0, 0, 0));
		
		void loadFromImageIntoSubArea(const Image& image, const Vector2i& subAreaBottomLeft);

		void loadFromAssetIntoSubArea(const std::string& assetPath, const Vector2i& subAreaBottomLeft, bool flipVertically = true);

		void enableMipmap(bool enable);

		void bind() const;

	protected:
		void updateSize(const sb::Vector2i & size);

		void createEmptyTexture(const Vector2i& size, const Color& color);

		void createGlTexture(const Vector2i& size, void* pixels);
		
		void activateMipmap();

		void deactivateMipmap();

	private:
		GLuint m_handle;

		bool m_isMipmapEnabled;

		bool m_isMipmapGenerated;

		Vector2i m_size;

		Vector2i m_internalSize;
	};
}