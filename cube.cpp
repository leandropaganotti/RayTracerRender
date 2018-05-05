#include "cube.h"

Cube::Cube(const Vector3f &color)
{
    c_diffuse = color;

    addVertex({0,0,0}); //0
    addVertex({1,0,0}); //1
    addVertex({0,0,-1});//2
    addVertex({1,0,-1});//3
    addVertex({0,1,0}); //4
    addVertex({1,1,0}); //5
    addVertex({0,1,-1});//6
    addVertex({1,1,-1});//7

    //bottom
    addTriangle(Triangle(0, 1, 3, {0,-1,0}));
    addTriangle(Triangle(0, 2, 3, {0,-1,0}));

    //top
    addTriangle(Triangle(4, 5, 7, {0,1,0}));
    addTriangle(Triangle(4, 6, 7, {0,1,0}));

    //front
    addTriangle(Triangle(0, 4, 5, {0,0,1}));
    addTriangle(Triangle(0, 1, 5, {0,0,1}));

    //back
    addTriangle(Triangle(2, 3, 7, {0,0,-1}));
    addTriangle(Triangle(2, 6, 7, {0,0,-1}));

    //right
    addTriangle(Triangle(1, 3, 7, {1,0,0}));
    addTriangle(Triangle(1, 5, 7, {1,0,0}));

    //left
    addTriangle(Triangle(0, 2, 6, {-1,0,0}));
    addTriangle(Triangle(0, 4, 6, {-1,0,0}));

//    for (size_t i=0; i < vertices.size(); ++i)
//    {
//        vertexBuffer[vertices[i]] += Vector3f(-0.5, -0.5, 0.5);
//    }
}

