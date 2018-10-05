#ifndef MODEL_H
#define MODEL_H
#include <string>
#include "mesh.h"

/**************************************************************
TODO: A model is going to be a collections of Mesh blocks,
which one with their material, transformation, and texture

current implementation loads a mesh from .obj file

**************************************************************/

class Model: public Mesh
{
public:
    Model(const Vector3 &color={1.0f});
    void loadFromFile(std::string path);
};

#endif // MODEL_H
