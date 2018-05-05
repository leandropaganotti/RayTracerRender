#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

class Mesh: public Object
{
    std::vector<size_t> vertices;
    std::vector<Triangle> triangles;
public:
    Mesh() = default;

    size_t addVertex(const Vertex& v);

    size_t addTriangle(const Triangle& tri);

    bool  intersection(const Ray& ray, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;

    friend std::ostream& operator << (std::ostream& os, const Mesh& m);
};

#endif // MESH_H

