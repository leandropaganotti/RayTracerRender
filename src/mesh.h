#ifndef MESH_H
#define MESH_H
#include <vector>
#include "plane.h"
#include "aabb.h"

class Mesh: public Object
{    
public:    
    Mesh(const Vector3f &color={1.0f});

    struct Triangle
    {
        size_t v0, v1, v2;      // 3 vertex indexes
        size_t nv0, nv1, nv2;   // 3 normal indexes for vertices
        Vector3f nf;            // face normal
        float area;
        bool intersection(const std::vector<Vector3f>   &vertices, const Ray &ray, float &tnear) const;
    };

    void addVertex(const Vector3f& v);
    void addNormal(const Vector3f& v);
    void addFace(size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2);

    void clear();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);
    friend std::ostream& operator << (std::ostream &os, const Triangle &f);

    // Object interface
public:
    bool  intersection(const Ray& ray, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    const Vector3f normal(const Vector3f &, size_t idx) const;

protected:
    AABB aabb;
    std::vector<Vector3f>   vertices;
    std::vector<Vector3f>   normals;
    std::vector<Triangle>   faces;
};



#endif // MESH_H
