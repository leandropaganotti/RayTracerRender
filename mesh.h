#ifndef MESH_H
#define MESH_H
#include <vector>
#include "plane.h"

class Vertex: public Vector3f
{
public:
    Vertex(float x): Vector3f(x){}
    Vertex(float x, float y, float z): Vector3f(x, y, z){}
    Vertex(const Vector3f &v): Vector3f(v){}
};

class Triangle
{
public:
    Triangle() = delete;
    static bool intersection(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Ray &ray, float &tnear);
};

class Face
{
public:
    Face(size_t v0, size_t v1, size_t v2, const Vector3f& normal);

    bool intersection(const Ray &ray, float &tnear) const;

    size_t v0, v1, v2;  // 3 vertex indexes
    Vector3f normal;    // normal

    friend std::ostream &operator <<(std::ostream &os, const Face &f);
};

class Mesh: public Object
{
protected:
    friend class Face;

    static std::vector<Vertex>   vertexBuffer;

    std::vector<size_t> vertices;
    std::vector<Face>   faces;

public:
    size_t addVertex(const Vertex& v);
    size_t addFace(const Face& t);

    bool  intersection(const Ray& ray, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    const Vector3f get(const Vector3f &, size_t idx) const
    {
        return faces[idx].normal;
    }

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);
    friend std::ostream& operator << (std::ostream &os, const Face &f);
};

inline
bool Face::intersection(const Ray &ray, float &tnear) const
{
    const Vector3f *p0 = &Mesh::vertexBuffer[ v0 ];
    const Vector3f *p1 = &Mesh::vertexBuffer[ v1 ];
    const Vector3f *p2 = &Mesh::vertexBuffer[ v2 ];

    return Triangle::intersection(*p0, *p1, *p2, ray, tnear);
}

#endif // MESH_H
