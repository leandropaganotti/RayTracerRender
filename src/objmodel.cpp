#include "objmodel.h"
#include "objparser.h"

void OBJModel::loadFromFile(const std::string &path)
{
    OBJParser::Parse(path, *this);
}
