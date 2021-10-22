#pragma once

#include "shape.h"
#include <memory>

class Primitive: public Shape
{
public:
    Primitive() = default;
    virtual ~Primitive() = default;

    virtual void getNormalAndUV(IntersectionData &isec) const = 0;
    virtual void getNormal(IntersectionData &isec) const = 0;
    virtual void getUV(IntersectionData &isec) const = 0;

};

template <typename T, typename ... Args>
std::shared_ptr<T> CreatePrimitive(Args&& ... args){
    return std::make_shared<T>(args...);
    //return std::make_shared<T>(std::forward<Args>(args)...);
    //return std::shared_ptr<T>(new T(std::forward<Args>(args) ... ));
}

namespace primitives
{
    extern std::shared_ptr<Primitive> Invisible;
    extern std::shared_ptr<Primitive> UnitCylinder;
    extern std::shared_ptr<Primitive> UnitBox;
    extern std::shared_ptr<Primitive> UnitSphere;
    extern std::shared_ptr<Primitive> XYPlane;
}
