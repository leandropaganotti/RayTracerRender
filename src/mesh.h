#ifndef MESH_H
#define MESH_H
#include <vector>
#include "plane.h"
#include "box.h"

class Mesh: public Object
{
    Box aabb;
public:    
    struct Triangle
    {
        size_t v0, v1, v2;      // 3 vertex indexes
        size_t nv0, nv1, nv2;   // 3 normal indexes for vertices
        Vector3f nf;            // face normal
        float area;
        bool intersection(const std::vector<Vector3f>   &vertices, const Ray &ray, float &tnear) const;
    };

    Mesh(const Vector3f &color={1.0f});

    void addVertex(const Vector3f& v);
    void addNormal(const Vector3f& v);
    void addFace(size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2);

    void clear();

    bool  intersection(const Ray& ray, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    const Vector3f normal(const Vector3f &, size_t idx) const;

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);
    friend std::ostream& operator << (std::ostream &os, const Triangle &f);

    std::vector<Vector3f>   vertices;
    std::vector<Vector3f>   normals;
    std::vector<Triangle>   faces;

    void computeAABB();

};



#endif // MESH_H
