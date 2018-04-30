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
    Mesh();

    size_t add(Vertex&& v);

    size_t add(const Vertex& v);

    size_t add(Triangle&& tri);

    size_t add(const Triangle& tri);

    bool  intersection(const Ray& ray, IntersectionData& inter) const;
    bool  intersection(const Ray& ray, float& dist) const;

    friend std::ostream& operator << (std::ostream& os, const Mesh& m);
};

#endif // MESH_H

