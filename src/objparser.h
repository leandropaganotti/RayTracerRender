#pragma once

#include <string>
#include "mesh.h"

class OBJParser
{
public:
    static void ParseMesh(std::shared_ptr<Mesh> &mesh , const std::string &path);
};
