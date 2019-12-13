#pragma once

#include "object.h"

class OBJModel: public Object
{    
public:
    OBJModel() = default;
    void loadFromFile(const std::string &path);    
};
