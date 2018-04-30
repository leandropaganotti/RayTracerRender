#include "cube.h"

Cube::Cube(const Vector3f &color)
{
    c_diffuse = color;

    add({0,0,0}); //0
    add({1,0,0}); //1
    add({0,0,-1});//2
    add({1,0,-1});//3
    add({0,1,0}); //4
    add({1,1,0}); //5
    add({0,1,-1});//6
    add({1,1,-1});//7

    //bottom
    add(Triangle(0, 1, 3, {0,-1,0}));
    add(Triangle(0, 2, 3, {0,-1,0}));

    //top
    add(Triangle(4, 5, 7, {0,1,0}));
    add(Triangle(4, 6, 7, {0,1,0}));

    //front
    add(Triangle(0, 4, 5, {0,0,1}));
    add(Triangle(0, 1, 5, {0,0,1}));

    //back
    add(Triangle(2, 3, 7, {0,0,-1}));
    add(Triangle(2, 6, 7, {0,0,-1}));

    //right
    add(Triangle(1, 3, 7, {1,0,0}));
    add(Triangle(1, 5, 7, {1,0,0}));

    //left
    add(Triangle(0, 2, 6, {-1,0,0}));
    add(Triangle(0, 4, 6, {-1,0,0}));

    for (size_t i=0; i < vertices.size(); ++i)
    {
        vertexBuffer[vertices[i]] += Vector3f(-0.5, -0.5, 0.5);
    }
}

