#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"

class Mesh: public Shape
{    
public:
    struct Triangle
    {
        size_t v0, v1, v2;      // 3 vertex indexes
        size_t nv0, nv1, nv2;   // 3 normal indexes for vertices
        Vector3 nf;             // face normal
        float area;
        bool intersection(const std::vector<Vector3>   &vertices, const Ray &ray, float &tnear) const;
    };

    void addVertex(const Vector3& v);
    void addNormal(const Vector3& v);
    void addFace(size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2);

    void clear();
    void updateAABB();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);
    friend std::ostream& operator << (std::ostream &os, const Triangle &f);

    // Shape interface
    bool  intersection(const Ray& ray, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    Vector3 normal(const Vector3 &phit, size_t idx) const;
    Vector2 uv(const Vector3 &, size_t) const;

protected:
    Mesh() = default;
    AABB aabb;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<Triangle>  faces;    

    friend class Shapes;
};
