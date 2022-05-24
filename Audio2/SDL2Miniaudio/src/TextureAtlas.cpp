#include "TextureAtlas.h"

using namespace std;
using namespace sb;

namespace blocks 
{
    sb::IntRect TextureAtlas::getTextureArea(size_t x, size_t y)
    {
        return IntRect(x * _textureSize.x, y * _textureSize.y,
            _textureSize.x, _textureSize.y);
    }
}
