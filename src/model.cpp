#include "model.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "objparser.h"

Model::Model(const Vector3f &color): Mesh(color)
{
}

void Model::load(std::string path)
{
    OBJParser::Parse(path, *this);
}
