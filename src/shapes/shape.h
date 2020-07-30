#pragma once

#include "intersection.h"
#include <utility>
#include <memory>


struct ShapeSample
{
    Vector3 p;
    Vector3 n;
    float  _1_pdf;
};

class Shape: public IntersectionIF
{
public:
    virtual void getIsecData(IntersectionData &isec) const = 0;
    virtual void getNormal(IntersectionData &isec) const = 0;
    virtual void getUV(IntersectionData &isec) const = 0;

    virtual void getSample(const Vector3 &, Vector3 &, float &) const {}
    virtual void getSample(IntersectionData &isec, ShapeSample &ss) const {}
    virtual float getArea() const {}
    virtual float getPdf(IntersectionData &isec, const Vector3 &wi) const {}

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
