#pragma once

#include "Window.h"
#include "GameAssets.h"
#include "Backdrop.h"
#include "Vector2.h"
#include "Board.h"

namespace blocks
{
    class Scene
    {
        sb::Window window;
        GameAssets gameAssets;
        Backdrop backdrop;
        Board board;
        sb::Vector2i size;

    public:
        Scene(size_t width, size_t height)
            : board(gameAssets, width, height),
            size(width, height)
        { }

        void start();

        void update();

        void draw();

        void run();
    };
}