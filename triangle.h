#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "plane.h"

class Triangle
{

    bool isInside2(const Vector3f& P) const;
    bool isInside(const Vector3f& P) const;
    bool intersection(const Ray &ray, Vector3f& phit, float &tnear) const;
public:

    Triangle(size_t v0, size_t v1, size_t v2, const Vector3f& normal);

    bool intersection(const Ray &ray, float &tnear) const;

    friend std::ostream& operator << (std::ostream& os, const Triangle& t);

    size_t v[3];        // 3 vertices
    Vector3f normal;    // normal
    float area;         // area
};

#endif // TRIANGLE_H
