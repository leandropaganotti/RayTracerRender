#ifndef MODEL_H
#define MODEL_H
#include <string>
#include "mesh.h"
#include "transformation.h"

class Model: public Mesh
{


public:
    Model();

    void load(std::string path);

    // Object interface
public:

};

#endif // MODEL_H
