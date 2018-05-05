#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

class Mesh: public Object
{
public:
    Mesh();

    size_t addVertex(const Vertex& v);

    size_t addTriangle(const Triangle& tri);

    bool  intersection(const Ray& ray, IntersectionData& inter) const;
    bool  intersection(const Ray& ray, float& tNear) const;

    friend std::ostream& operator << (std::ostream& os, const Mesh& m);

    std::vector<size_t> vertices;
    std::vector<Triangle> triangles;
};

#endif // MESH_H

