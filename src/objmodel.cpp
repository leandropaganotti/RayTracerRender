#include "objmodel.h"
#include "objparser.h"

void OBJModel::loadFromFile(const std::string &path)
{
    shape = OBJParser::ParseMesh(path);
}
