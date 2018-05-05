#ifndef MESH_H
#define MESH_H
#include <vector>
#include "plane.h"


class Triangle
{
//    bool isInside2(const Vector3f& P) const;
//    bool isInside(const Vector3f& P) const;
//    bool intersection(const Ray &ray, Vector3f& phit, float &tnear) const;
public:

    Triangle(size_t v0, size_t v1, size_t v2, const Vector3f& normal);

    friend std::ostream& operator << (std::ostream& os, const Triangle& t);

    size_t v[3];        // 3 vertex indexes
    Vector3f normal;    // normal
    float area;         // area

    static bool intersection(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Ray &ray, float &tnear);
};


class Vertex;

class Mesh: public Object
{
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;

    bool triangleIntersection(size_t idx, const Ray &ray, float &tnear) const;

public:
    Mesh() = default;

    size_t addVertex(const Vertex& v);

    size_t addTriangle(const Triangle& tri);

    bool  intersection(const Ray& ray, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;

    friend std::ostream& operator << (std::ostream& os, const Mesh& m);
};


class Vertex: public Vector3f
{
public:
    Vertex(float x): Vector3f(x){}
    Vertex(float x, float y, float z): Vector3f(x, y, z){}
    Vertex(const Vector3f &v): Vector3f(v){}
};

class VertexBuffer: public std::vector<Vertex>
{
public:
    friend std::ostream& operator << (std::ostream& os, const VertexBuffer& vb);
};



#endif // MESH_H

