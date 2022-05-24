#include "Block.h"
#include "Rect.h"
#include "Logger.h"
#include "GameAssets.h"

using namespace sb;

namespace blocks
{
    Block::Block(TextureAtlas& blockTextures, BlockType type)
        : _blockTextures(blockTextures)
    {
        setType(type);
    }

    void Block::setType(BlockType type)
    {
        Vector2i texturePosition = BlockTypeHelper::getTexturePosition(type);
        setTexture(_blockTextures.getTextureSheet(), _blockTextures.getTextureArea(texturePosition));
    }
}
