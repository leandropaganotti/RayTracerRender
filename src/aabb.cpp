#include "aabb.h"
#include <float.h>

Vector3f AABB::getPosition()
{
    return Vector3f(transformation[0][3], transformation[1][3], transformation[2][3]);
}

void AABB::create(const std::vector<Vector3f> &vertices)
{
    float minX=FLT_MAX, maxX=FLT_MIN;
    float minY=FLT_MAX, maxY=FLT_MIN;
    float minZ=FLT_MAX, maxZ=FLT_MIN;

    for(size_t i= 0 ; i < vertices.size(); ++i)
    {
        if (vertices[i].x < minX)
            minX = vertices[i].x;
        if (vertices[i].y < minY)
            minY = vertices[i].y;
        if (vertices[i].z < minZ)
            minZ = vertices[i].z;

        if (vertices[i].x > maxX)
            maxX = vertices[i].x;
        if (vertices[i].y > maxY)
            maxY = vertices[i].y;
        if (vertices[i].z > maxZ)
            maxZ = vertices[i].z;
    }
    Vector3f translate, scale;

    scale.x = maxX - minX;
    scale.y = maxY - minY;
    scale.z = maxZ - minZ;

    translate.x =  (maxX + minX) / 2.0f;
    translate.y =  (maxY + minY) / 2.0f;
    translate.z =  (maxZ + minZ) / 2.0f;

    setTransformation(translate, Vector3f(0.0f), scale);
}

void AABB::setTransformation(const Vector3f &translate, const Vector3f &rotate, const Vector3f &scale)
{
    Box::setTransformation(translate, rotate, scale);
}

void AABB::setTransformation(const Matrix4x4f &transformation)
{
    Box::setTransformation(transformation);
}
