#pragma once

#include <string>
#include "mesh.h"

class OBJParser
{
public:
    static std::shared_ptr<Mesh> ParseMesh(const std::string &path);
};
