#include "mesh.h"
#include <float.h>
#include "material.h"
#include "invisible.h"
#include "bvh.h"

/************************************************************************
 * Mesh class
 ************************************************************************/

std::shared_ptr<Mesh> Mesh::Create()
{
    return std::shared_ptr<Mesh>(new Mesh);
}

void Mesh::addVertex(const Vector3 &v)
{
    vertices.push_back(v);
}

void Mesh::addNormal(const Vector3 &n)
{
    normals.push_back(n);
}

void Mesh::clear()
{
    vertices.clear();
    normals.clear();
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

Mesh::Mesh()
{

}
