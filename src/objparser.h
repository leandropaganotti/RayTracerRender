#pragma once

#include <string>
#include "mesh.h"

class OBJParser
{
public:
    static void ParseMesh(const std::string &path, std::shared_ptr<Mesh> &mesh);
};
