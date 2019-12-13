#include "aabb.h"
#include <float.h>

Vector3 AABB::getCenter()
{
    return (min + max ) * 0.5f;
}


void AABB::create(const std::vector<Vector3> &vertices)
{
    min.x=FLT_MAX, max.x=FLT_MIN;
    min.y=FLT_MAX, max.y=FLT_MIN;
    min.z=FLT_MAX, max.z=FLT_MIN;

    for(size_t i= 0 ; i < vertices.size(); ++i)
    {
        if (vertices[i].x < min.x)
            min.x = vertices[i].x;
        if (vertices[i].y < min.y)
            min.y = vertices[i].y;
        if (vertices[i].z < min.z)
            min.z = vertices[i].z;

        if (vertices[i].x > max.x)
            max.x = vertices[i].x;
        if (vertices[i].y > max.y)
            max.y = vertices[i].y;
        if (vertices[i].z > max.z)
            max.z = vertices[i].z;
    }    
}

AABB::AABB(const Vector3 &min, const Vector3 &max):
    AABox(min, max)
{

}
