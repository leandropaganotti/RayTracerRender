#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "mesh.h"
#include <string>

class OBJParser
{
public:
    OBJParser() = default;
    static Mesh* Parse(std::string path);
    static void Parse(std::string path, Mesh &mesh);
};

#endif // OBJPARSER_H
