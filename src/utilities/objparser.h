#pragma once

#include <string>
#include <memory>

class Mesh;

class OBJParser
{
public:
    static std::shared_ptr<Mesh> ParseMesh(const std::string &filepath);
};
