#pragma once

#include "intersection.h"
#include <utility>
#include <memory>

class Shape: public IntersectionIF
{
public:
    virtual void getIsecData(IntersectionData &isec) const = 0;
    virtual void getNormal(IntersectionData &isec) const = 0;
    virtual void getUV(IntersectionData &isec) const = 0;

    virtual ~Shape(){};
};

template <typename T, typename ... Args>
std::shared_ptr<T> CreateShape(Args&& ... args){
    return std::make_shared<T>(args...);
    //return std::make_shared<T>(std::forward<Args>(args)...);
    //return std::shared_ptr<T>(new T(std::forward<Args>(args) ... ));
}

namespace shape
{
    extern std::shared_ptr<Shape> Invisible;
    extern std::shared_ptr<Shape> unitCylinder;
    extern std::shared_ptr<Shape> unitBox;
    extern std::shared_ptr<Shape> unitSphere;
    extern std::shared_ptr<Shape> xyPlane;
}
