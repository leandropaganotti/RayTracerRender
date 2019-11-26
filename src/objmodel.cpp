#include "objmodel.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "objparser.h"

void OBJModel::loadFromFile(std::string path)
{
    OBJParser::Parse(path, *this);
    aabb.create(vertices); // create AABB
}
