#include "cube.h"

Cube::Cube(const Vector3f &color)
{
    c_diffuse = color;

    size_t v0 = addVertex({0,0,0}); //0
    size_t v1 = addVertex({1,0,0}); //1
    size_t v2 = addVertex({0,0,-1});//2
    size_t v3 = addVertex({1,0,-1});//3
    size_t v4 = addVertex({0,1,0}); //4
    size_t v5 = addVertex({1,1,0}); //5
    size_t v6 = addVertex({0,1,-1});//6
    size_t v7 = addVertex({1,1,-1});//7

    //bottom
    addFace(Face(v0, v1, v3, {0,-1,0}));
    addFace(Face(v0, v2, v3, {0,-1,0}));

    //top
    addFace(Face(v4, v5, v7, {0,1,0}));
    addFace(Face(v4, v6, v7, {0,1,0}));

    //front
    addFace(Face(v0, v4, v5, {0,0,1}));
    addFace(Face(v0, v1, v5, {0,0,1}));

    //back
    addFace(Face(v2, v3, v7, {0,0,-1}));
    addFace(Face(v2, v6, v7, {0,0,-1}));

    //right
    addFace(Face(v1, v3, v7, {1,0,0}));
    addFace(Face(v1, v5, v7, {1,0,0}));

    //left
    addFace(Face(v0, v2, v6, {-1,0,0}));
    addFace(Face(v0, v4, v6, {-1,0,0}));
}

