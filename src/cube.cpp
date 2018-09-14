#include "cube.h"
#include "utils.h"

Cube::Cube(const Vector3f &center, const Vector3f &size, const Vector3f &rotation, const Vector3f& color):
	Mesh(color)
{
//    size_t v0 = addVertex({-0.5,-0.5, 0.5}); //0
//	size_t v1 = addVertex({ 0.5,-0.5, 0.5}); //1
//	size_t v2 = addVertex({-0.5,-0.5,-0.5}); //2
//	size_t v3 = addVertex({ 0.5,-0.5,-0.5}); //3
//	size_t v4 = addVertex({-0.5, 0.5, 0.5}); //4
//	size_t v5 = addVertex({ 0.5, 0.5, 0.5}); //5
//	size_t v6 = addVertex({-0.5, 0.5,-0.5}); //6
//	size_t v7 = addVertex({ 0.5, 0.5,-0.5}); //7

//    //bottom
//    addFace(Face(v0, v1, v3, {0,-1,0}));
//    addFace(Face(v0, v2, v3, {0,-1,0}));

//    //top
//    addFace(Face(v4, v5, v7, {0,1,0}));
//    addFace(Face(v4, v6, v7, {0,1,0}));

//    //front
//    addFace(Face(v0, v4, v5, {0,0,1}));
//    addFace(Face(v0, v1, v5, {0,0,1}));

//    //back
//    addFace(Face(v2, v3, v7, {0,0,-1}));
//    addFace(Face(v2, v6, v7, {0,0,-1}));

//    //right
//    addFace(Face(v1, v3, v7, {1,0,0}));
//    addFace(Face(v1, v5, v7, {1,0,0}));

//    //left
//    addFace(Face(v0, v2, v6, {-1,0,0}));
//    addFace(Face(v0, v4, v6, {-1,0,0}));


//    for (auto &idx : vertices)
//    {
//        vertexBuffer[idx] = T(center) * Rz(deg2rad(rotation.z)) * Ry(deg2rad(rotation.y)) * Rx(deg2rad(rotation.x)) * S(size) * vertexBuffer[idx];
//    }
}

