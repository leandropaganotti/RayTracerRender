#ifndef BOX_H
#define BOX_H

#include "object.h"
#include "matrix.h"
#include "model.h"

class Box : public Object
{
public:
    Box();
    // Object interface
    bool intersection(const Ray &ray, IntersectionData &isec) const;
    bool intersection(const Ray &ray, float &tnear) const;
    const Vector3f normal(const Vector3f &, size_t idx) const;

    ModelMatrix& getModel()
    {
        return model;
    }

private:
    Vector3f min;
    Vector3f max;

    ModelMatrix model;

};

#endif // BOX_H
