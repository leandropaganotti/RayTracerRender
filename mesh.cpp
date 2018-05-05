#include "mesh.h"
#include <float.h>

size_t Mesh::addVertex(const Vertex &v)
{
    vertexBuffer.push_back(v);
    vertices.push_back(vertexBuffer.size() - 1);
    return vertexBuffer.size() - 1;
}

size_t Mesh::addTriangle(const Triangle &tri)
{
    triangles.push_back(tri);
    return triangles.size() - 1;
}


bool Mesh::intersection(const Ray& ray, IntersectionData &isec) const
{   
    float tnear;
    isec.tnear = FLT_MAX;
    size_t tri;
    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangles[i].intersection(ray, tnear))
        {
            if (tnear < isec.tnear)
            {
                isec.tnear = tnear;
                tri = i;
            }
        }
    }
    if (isec.tnear < FLT_MAX)
    {
        isec.normal = triangles[tri].normal;
        isec.object = this;
        isec.phit   = ray.origin + isec.tnear * ray.direction;
        return true;
    }
    return false;
}

bool Mesh::intersection(const Ray& ray, float &tnear) const
{
    float t;
    tnear = FLT_MAX;
    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangles[i].intersection(ray, t))
        {
            if (t < tnear) tnear = t;
        }
    }
    return tnear < FLT_MAX ? true : false;
}

std::ostream &operator <<(std::ostream &os, const Mesh &m)
{
    os << "Mesh:" << std::endl;
    os << "|Vertices: " << std::endl;
    for(size_t i= 0 ; i < m.vertices.size(); ++i)
        std::cout << "||" << m.vertices[i] << "->" << vertexBuffer[m.vertices[i]] << std::endl;

    os << "|Triangles: " << std::endl;
    for(size_t i= 0 ; i < m.triangles.size(); ++i)
        std::cout << "||" << m.triangles[i] << std::endl;

    return os ;
}

