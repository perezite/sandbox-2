#include "GameAssets.h"

using namespace std;

namespace blocks
{
    GameAssets::GameAssets() :
        blockTextureSheet("Textures/Blocks.png"),
        squareTextureSheet("Textures/Blocks.png"),
        blockTextureAtlas(blockTextureSheet, 128),
        squareTextureAtlas(squareTextureSheet, 128)
    { }
}
