#include "mesh.h"
#include <float.h>

Mesh::Mesh()
{

}

size_t Mesh::addVertex(const Vertex &v)
{
    std::cout << "not move "<< std::endl;
    vertexBuffer.push_back(v);
    vertices.push_back(vertexBuffer.size() - 1);
    return vertexBuffer.size() - 1;
}

size_t Mesh::addTriangle(const Triangle &tri)
{
    triangles.push_back(tri);
    return triangles.size() - 1;
}


bool Mesh::intersection(const Ray& ray, IntersectionData &inter) const
{   
    float tNear;
    inter.tNear = FLT_MAX;
    size_t tri;
    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangles[i].intersection(ray, tNear))
        {
            if (tNear < inter.tNear)
            {
                inter.tNear = tNear;
                tri = i;
            }
        }
    }
    if (inter.tNear < FLT_MAX)
    {
        inter.normal = triangles[tri].normal;
        inter.object = this;
        inter.phit   = ray.origin + inter.tNear * ray.direction;
        return true;
    }
    return false;
}

bool Mesh::intersection(const Ray& ray, float &tNear) const
{
    float t;
    tNear = FLT_MAX;
    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangles[i].intersection(ray, t))
        {
            if (t < tNear) tNear = t;
        }
    }
    return tNear < FLT_MAX ? true : false;
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

