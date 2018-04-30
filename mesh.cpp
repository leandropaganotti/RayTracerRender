#include "mesh.h"
#include <float.h>

Mesh::Mesh()
{

}

size_t Mesh::add(Vertex &&v)
{
    vertexBuffer.push_back(v);
    vertices.push_back(vertexBuffer.size() - 1);
    return vertexBuffer.size() - 1;
}

size_t Mesh::add(const Vertex &v)
{
    vertexBuffer.push_back(v);
    vertices.push_back(vertexBuffer.size() - 1);
    return vertexBuffer.size() - 1;
}

size_t Mesh::add(Triangle &&tri)
{
    triangles.push_back(tri);
    return triangles.size() - 1;
}

size_t Mesh::add(const Triangle &tri)
{
    triangles.push_back(tri);
    return triangles.size() - 1;
}

inline
bool Mesh::intersection(const Ray& ray, IntersectionData &inter) const
{
    float phitDist = FLT_MAX, dist;
    Vector3f phit;
    size_t phitTriangle;

    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangles[i].intersection(ray, phit, dist))
        {
            if (dist < phitDist)
            {
                inter.phit = phit;
                phitDist = dist;
                phitTriangle = i;
            }
        }
    }
    if (phitDist < FLT_MAX)
    {
        inter.dist   = phitDist;
        inter.normal = triangles[phitTriangle].normal;
        inter.object = &triangles[phitTriangle];
        return true;
    }
    else
        return false;
}

bool Mesh::intersection(const Ray& ray, float &dist) const
{
    float phitDist = FLT_MAX, d;
    Vector3f phit;

    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangles[i].intersection(ray, phit, d))
        {
            if (d < phitDist)
            {
                phitDist = d;
            }
        }
    }
    if (phitDist < FLT_MAX)
    {
        dist = phitDist;
        return true;
    }
    else
        return false;
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

