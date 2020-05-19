#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include "instance.h"
#include <map>

 class BVH;
 class Mesh;

class QuadMesh: public Shape, public AABBIF
{   
public:
    QuadMesh() = default;
    QuadMesh(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t v3, size_t nv0, size_t nv1, size_t nv2, size_t nv3);

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;

    size_t v[4];
    size_t nv[4];
    Vector3 nf;

    float area;

    friend std::ostream& operator << (std::ostream &os, const QuadMesh &q);

    AABB getAABB() const override;

    AABB aabb;

private:
    const Mesh *mesh;

    //ignore those
    Vector3 normal(const Vector3 &, size_t ) const override;
    Vector2 uv(const Vector3 &, size_t) const override;
    void fetch(const Ray &, IntersectionData &) const override;
};

class TriangleMesh: public Shape, public AABBIF
{
public:
    TriangleMesh() = default;
    TriangleMesh(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2 );

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;

    size_t v[3];    // vertices index
    size_t nv[3];   // normals index

    Vector3 nf;

    float area;

    friend std::ostream& operator << (std::ostream &os, const TriangleMesh &t);

    AABB getAABB() const override;

    AABB aabb;

    size_t idx;

private:
    //pointer to the mesh
    const Mesh *mesh;

    //ignore those
    Vector3 normal(const Vector3 &, size_t ) const override;
    Vector2 uv(const Vector3 &, size_t) const override;
    void fetch(const Ray &, IntersectionData &) const override;
};


class Mesh: public Shape
{    
public:
    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(const TriangleMesh& f);

    void clear();
    void updateAABB();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;
    Vector3 normal(const Vector3 &phit, size_t idx) const override;
    Vector2 uv(const Vector3 &, size_t) const override;
    virtual void fetch(const Ray &ray, IntersectionData &isec) const override;

protected:
    Shape   *bvh;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<TriangleMesh>  faces;

    friend class TriangleMesh;
    friend class QuadMesh;
};

class GMesh: public Instance
{
public:
    GMesh(std::shared_ptr<Mesh> mesh);

    void fetch(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};


class BVH: public Shape
{
public:
    ~BVH();

    static Shape* Create(std::vector<TriangleMesh> &shapes, size_t l, size_t r, size_t axis);
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
private:
    BVH();
    AABB aabb;
    Shape *left;
    Shape *right;

    static size_t qsplit(std::vector<TriangleMesh> &shapes, size_t l, size_t r, float pivot, size_t axis);

    Vector3 normal(const Vector3 &phit, size_t idx) const override;
    Vector2 uv(const Vector3 &phit, size_t idx) const override;
    void fetch(const Ray &ray, IntersectionData &isec) const override;
};
