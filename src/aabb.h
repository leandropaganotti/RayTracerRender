#ifndef AABB_H
#define AABB_H

#include "box.h"

class AABB: protected Box
{
public:
    AABB() = default;
    virtual ~AABB() = default;

    virtual void create(const std::vector<Vector3f>& vertices);
    virtual Vector3f getPosition();

    // Object interface
public:
    bool intersection(const Ray &ray, float &tnear) const;

    // Transformation interface
public:
    void setTransformation(const Vector3f &translate, const Vector3f &rotate, const Vector3f &scale);
    void setTransformation(const Matrix4x4f &transformation);
};

inline
bool AABB::intersection(const Ray &ray, float &tnear) const
{
    return Box::intersection(ray, tnear);
}



#endif // AABB_H
