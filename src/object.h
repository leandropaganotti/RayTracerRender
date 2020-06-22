#pragma once

#include "shape.h"
#include <memory>
#include "material.h"

class Object
{
    std::shared_ptr<Shape> shape;
    std::vector<std::shared_ptr<Material>> material;

    void getMaterial(IntersectionData &isec)
    {
        isec.material = material[isec.idx].get();
    }

    bool Intersection();
};
