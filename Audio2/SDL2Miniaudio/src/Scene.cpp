#include "Scene.h"
#include "Input.h"
#include "Logger.h"
#include "Stats.h"
#include "VectorHelper.h"
#include <iostream>

using namespace sb;
using namespace std;

namespace blocks
{    
    void Scene::start()
    {
        window.setSize(300, 540);
        window.setFramerateLimit(65);
        window.getCamera().requestSize(toVector2f(size));
        window.getCamera().setCenter(.5f * window.getCamera().getSize());
    }

    void Scene::update()
    {
        Input::update();
        window.update();
        backdrop.update(window.getCamera());
        board.update(window);
    }

    void Scene::draw()
    {
        window.draw(backdrop);
        window.draw(board);
    }

    void Scene::run()
    {
        start();

        while (window.isOpen()) {
            update();
            window.clear(Color(1, 1, 1, 1));   
            draw();
            window.display();
            printStats();
        }
    }
}
