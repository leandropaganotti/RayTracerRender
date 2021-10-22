#include "mesh.h"
#include <float.h>
#include "material.h"
#include "invisible.h"
#include "bvh.h"
#include "triangle.h"
#include "objectvector.h"

/************************************************************************
 * Mesh class
 ************************************************************************/
Mesh::Mesh()
{
    aggregate = std::make_shared<BVH>();
}

Mesh::Mesh(const std::shared_ptr<Aggregate> &agg): aggregate(agg)
{
}

void Mesh::addVertex(const Vector3 &v)
{
    vertices.push_back(v);
}

void Mesh::addNormal(const Vector3 &n)
{
    normals.push_back(n);
}

void Mesh::addFace(const std::shared_ptr<MeshTriangle> &face)
{
    face->idx = faces.size();
    faces.push_back(face);
}

void Mesh::buildAggregate()
{
    aggregate->build(faces);
}

std::ostream &operator <<(std::ostream &os, const Mesh &m)
{
    os << "Mesh:" << std::endl;
    os << "|Vertices: " << std::endl;
    for(size_t i= 0 ; i < m.vertices.size(); ++i)
        std::cout << "||" << i << ":" << m.vertices[i] << std::endl;

    os << "|Normals: " << std::endl;
    for(size_t i= 0 ; i < m.normals.size(); ++i)
        std::cout << "||" << i << ":" << m.normals[i] << std::endl;

//    os << "|Faces: " << std::endl;
//    for(size_t i= 0 ; i < m.faces.size(); ++i)
//        std::cout << "||" << i << ":" << m.faces[i] << std::endl;

    return os ;
}
