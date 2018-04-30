#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"
#include "plane.h"

class Triangle: public Object
{
protected:

    bool isInside2(const Vector3f& P) const;
    bool isInside(const Vector3f& P) const;

public:

    Triangle(size_t v0, size_t v1, size_t v2, const Vector3f& normal, const Vector3f& color={1.0f});

    bool intersection(const Ray& ray, IntersectionData& inter) const;
    bool intersection(const Ray& ray, float& dist) const;

    bool intersection(const Ray &ray, Vector3f& phit, float &dist) const;

    friend std::ostream& operator << (std::ostream& os, const Triangle& t);

    size_t v[3];        // 3 vertices
    Vector3f normal;    // normal
    float area;         // area
};

#endif // TRIANGLE_H
