#include "Backdrop.h"
#include "DrawTarget.h"

namespace blocks 
{ 
    void Backdrop::update(sb::Camera& camera) 
    {
        static sb::Color bottomColor = sb::Color::createFromRGB(252, 182, 159);
        static sb::Color topColor = sb::Color::createFromRGB(255, 236, 210);
        const sb::Vector2f& size = camera.getSize();
        _mesh[0] = sb::Vertex(sb::Vector2f(-size.x, -size.y), bottomColor);
        _mesh[1] = sb::Vertex(sb::Vector2f(+size.x, -size.y), bottomColor);
        _mesh[2] = sb::Vertex(sb::Vector2f(-size.x, +size.y), topColor);
        _mesh[3] = sb::Vertex(sb::Vector2f(+size.x, +size.y), topColor);
        setPosition(camera.getCenter());
    }

    void Backdrop::draw(sb::DrawTarget& target, sb::DrawStates states) 
    {
        states.transform *= getTransform();
        target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
    }
}

