#ifndef OBJMODEL_H
#define OBJMODEL_H
#include <string>
#include "mesh.h"

/**************************************************************
TODO: A model is going to be a collections of Mesh blocks,
which one with their material, transformation, and texture

current implementation loads a mesh from .obj file

**************************************************************/

class OBJModel: public Mesh
{
public:
    OBJModel() = default;
    void loadFromFile(std::string path);
};

#endif // OBJMODEL_H
