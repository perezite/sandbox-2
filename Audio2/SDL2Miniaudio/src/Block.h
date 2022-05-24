#pragma once

#include "Sprite.h"
#include "Rect.h"
#include "Texture.h"
#include "GameAssets.h"
#include "TextureAtlas.h"
#include "BlockType.h"

namespace blocks
{

    class Block : public sb::Sprite
    {
        TextureAtlas& _blockTextures;

    public:
        Block(TextureAtlas& blockTextures, BlockType type = BlockType::I);

        void setType(BlockType type);
    };

}