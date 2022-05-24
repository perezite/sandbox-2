#pragma once

#include "Texture.h"
#include "Vector2.h"

namespace blocks
{
    enum class BlockType
    {
        T, I, Simple, VerySimple
    };

    class BlockTypeHelper
    {
    public:
        static inline sb::Vector2i getTexturePosition(BlockType type)
        {
            if (type == BlockType::I)
                return sb::Vector2i(1, 2);
            else if (type == BlockType::T)
                return sb::Vector2i(0, 0);
            else if (type == BlockType::Simple)
                return sb::Vector2i(0, 0);
            else if (type == BlockType::VerySimple)
                return sb::Vector2i(0, 0);
            
            SB_ERROR("Invalid block type " << (int)type);
            return sb::Vector2i();
        }
    };
}
