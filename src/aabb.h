#ifndef AABB_H
#define AABB_H

#include "box.h"

class AABB: protected Box
{
public:
    AABB() = default;
    virtual ~AABB() = default;

    virtual void create(const std::vector<Vector3>& vertices);
    virtual Vector3 getPosition();

    // Object interface
public:
    bool intersection(const Ray &ray, float &tnear) const;

    // Transformation interface
public:
    void setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale);
    void setTransformation(const Matrix4 &transformation);
};

inline
bool AABB::intersection(const Ray &ray, float &tnear) const
{
    return Box::intersection(ray, tnear);
}



#endif // AABB_H
