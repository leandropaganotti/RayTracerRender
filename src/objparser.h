#pragma once

#include <string>
#include "objmodel.h"

class OBJParser
{
public:
    static void Parse(const std::string &path, OBJModel &model);
};
