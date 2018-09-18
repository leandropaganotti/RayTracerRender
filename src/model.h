#ifndef MODEL_H
#define MODEL_H
#include <string>
#include "mesh.h"

class Model: public Mesh
{
public:
    Model(const Vector3f &color={1.0f});
    void load(std::string path);
};

#endif // MODEL_H
