#include "cube.h"

Cube::Cube(const Vector3f &color)
{

    add({0,0,0}); //0
    add({1,0,0}); //1
    add({0,0,-1});//2
    add({1,0,-1});//3
    add({0,1,0}); //4
    add({1,1,0}); //5
    add({0,1,-1});//6
    add({1,1,-1});//7

    //bottom
    add(Triangle(0, 1, 3, {0,-1,0}, color));
    add(Triangle(0, 2, 3, {0,-1,0}, color));

    //top
    add(Triangle(4, 5, 7, {0,1,0}, color));
    add(Triangle(4, 6, 7, {0,1,0}, color));

    //front
    add(Triangle(0, 4, 5, {0,0,1}, color));
    add(Triangle(0, 1, 5, {0,0,1}, color));

    //back
    add(Triangle(2, 3, 7, {0,0,-1}, color));
    add(Triangle(2, 6, 7, {0,0,-1}, color));

    //right
    add(Triangle(1, 3, 7, {1,0,0}, color));
    add(Triangle(1, 5, 7, {1,0,0}, color));

    //left
    add(Triangle(0, 2, 6, {-1,0,0}, color));
    add(Triangle(0, 4, 6, {-1,0,0}, color));
}

