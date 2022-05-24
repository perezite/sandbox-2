#include "CameraDemo1.h"
#include "Input.h"
#include "Quad.h"
#include "Window.h"

using namespace sb;
using namespace std;

namespace cameraDemo1
{
    void demo0()
    {
        Window window(400, 600);
        Quad quad1;
        Quad quad2;

        quad1.setScale(1, 1);
        quad2.setScale(0.5f);
        quad2.setPosition(-4, -4);
        window.getCamera().requestSize(10, 10);
        window.getCamera().setCenter(-4, -4);

        Vector2f actualCameraSize = window.getCamera().getSize();
        cout << actualCameraSize.x << " " << actualCameraSize.y << endl;

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(quad1);
            window.draw(quad2);
            window.display();
        }
    }

    void resize1(Window& window)
    {
        if (window.getSize() == Vector2i(400, 400))
            window.setSize(400, 800);
        else
            window.setSize(400, 400);
    }

    void print1(Window& window) {
        const Vector2f& requestedCameraSize = window.getCamera().getRequestedSize();
        const Vector2f& cameraSize = window.getCamera().getSize();
        const Vector2i& windowSize = window.getSize();
        cout << "requested: " << requestedCameraSize.x << " " << requestedCameraSize.y << endl;
        cout << "actual: " << cameraSize.x << " " << cameraSize.y << endl;
        cout << "window: " << windowSize.x << " " << windowSize.y << endl;
    }

    void demo1() 
    {
        Window window(400, 800);
        Quad quad;

        window.getCamera().requestSize(2, 4);
        print1(window);

        quad.setScale(1, 1);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            if (Input::isKeyGoingDown(KeyCode::Return))
                resize1(window);
            if (Input::isWindowResizing(window))
                print1(window);
          
            window.clear(Color(1, 1, 1, 1));
            window.draw(quad);
            window.display();
        }
    }

    void run()
    {
        demo1();
        // demo0();
    }
}
