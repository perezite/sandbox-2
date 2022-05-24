#pragma once

#include "Texture.h"
#include "TextureAtlas.h"
#include <string>

namespace blocks
{
    struct GameAssets 
    {
        sb::Texture blockTextureSheet;

        sb::Texture squareTextureSheet;

    public:
        GameAssets();

        blocks::TextureAtlas blockTextureAtlas;

        blocks::TextureAtlas squareTextureAtlas;
    };
}
