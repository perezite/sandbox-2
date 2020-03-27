#pragma once
#include "SDL.h"
#include "Vector2.h"
#include <string>

namespace sb
{
	class Image
	{
	public:
		Image(const std::string& assetPath, bool flipVertically = true);

		virtual ~Image();

		inline void* getPixels() const { return m_surface->pixels;  }

		inline const sb::Vector2i getSize() const { return sb::Vector2i(m_surface->w, m_surface->h); }

		inline const int getWidth() const { return m_surface->w; }

		inline const int getHeight() const { return m_surface->h; }

	protected:
		SDL_Surface* convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat);

		SDL_Surface* flipSurface(SDL_Surface* surface);

		void flipSurfacePixels(SDL_Surface* destination, SDL_Surface* target);

	private:
		SDL_Surface* m_surface;
	};
}