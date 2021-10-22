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
    faces = std::make_shared<BVH<MeshTriangle>>();
}

Mesh::Mesh(const std::shared_ptr<Aggregate<MeshTriangle> > &agg): faces(agg)
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
    faces->add(face);
}

void Mesh::buildAggregate()
{
    faces->build();
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
