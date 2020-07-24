#include "Image.h"
#include "Asset.h"
#include <SDL2/SDL_image.h>

namespace sb
{
	Image::Image(const std::string & assetPath, bool flipVertically)
	{
		std::string filePath = Asset::getFilePath(assetPath);
		m_surface = IMG_Load(filePath.c_str());
		SB_ERROR_IF(m_surface == NULL, IMG_GetError());
		m_surface = convertPixelFormat(m_surface, SDL_PIXELFORMAT_ABGR8888);
		if (flipVertically)
			m_surface = flipSurface(m_surface);
	}

	Image::~Image()
	{
		SDL_FreeSurface(m_surface);
	}
	
	SDL_Surface* Image::convertPixelFormat(SDL_Surface* surface, Uint32 pixelFormat)
	{
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, SDL_SWSURFACE);
		SDL_CHECK(converted);
		SDL_FreeSurface(surface);
		return converted;
	}

	SDL_Surface* Image::flipSurface(SDL_Surface* surface)
	{
		SDL_Surface *flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		SB_ERROR_IF(flipped == NULL, SDL_GetError());

		if (SDL_MUSTLOCK(flipped)) SDL_CHECK(SDL_LockSurface(flipped) < 0);
		flipSurfacePixels(surface, flipped);
		if (SDL_MUSTLOCK(flipped)) SDL_UnlockSurface(flipped);

		SDL_FreeSurface(surface);
		return flipped;
	}

	void Image::flipSurfacePixels(SDL_Surface* destination, SDL_Surface* target)
	{
		for (int row = destination->h - 1; row >= 0; row--) {
			for (int col = 0; col < destination->w; col++) {
				size_t sourceOffset = row * destination->w + col;
				size_t sourceOffsetBytes = sourceOffset * destination->format->BytesPerPixel;
				size_t destOffset = (destination->h - row - 1) * destination->w + col;
				size_t destOffsetBytes = destOffset * destination->format->BytesPerPixel;

				memcpy((char*)target->pixels + destOffsetBytes,
					(char*)destination->pixels + sourceOffsetBytes, destination->format->BytesPerPixel);
			}
		}
	}
}
