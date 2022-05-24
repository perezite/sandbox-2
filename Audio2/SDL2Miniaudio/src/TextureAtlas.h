#pragma once

#include "Texture.h"
#include "Vector2.h"
#include "Rect.h"

namespace blocks 
{
	class TextureAtlas
	{
		sb::Texture& _textureSheet;

		const sb::Vector2ui _textureSize;

	public:
		TextureAtlas(sb::Texture& textureSheet, const sb::Vector2ui& textureSize)
			: _textureSheet(textureSheet), _textureSize(textureSize)
		{ }

		inline sb::Texture& getTextureSheet() { return _textureSheet; }

		sb::IntRect getTextureArea(size_t x, size_t y);

		inline sb::IntRect getTextureArea(const sb::Vector2i& position) { return getTextureArea(position.x, position.y); }
	};
}