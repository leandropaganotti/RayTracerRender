#ifndef CUBE_H
#define CUBE_H

#include "mesh.h"

class Cube: public Mesh
{
public:
    Cube(const Vector3f &center={0.0f}, const Vector3f &size={1.0f}, const Vector3f &rotation={0.0f}, const Vector3f& color={1.0f});
};

#endif // CUBE_H
