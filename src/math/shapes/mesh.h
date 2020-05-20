#pragma once

#include <vector>
#include "shape.h"
#include "instance.h"
#include "aabb.h"

 class BVH;
 class MeshFace;

class Mesh: public ShapeNormalUV
{    
public:
    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(std::shared_ptr<MeshFace> face);

    void clear();
    void buildBoundingVolume();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;
    Vector3 getNormal(const Vector3 &phit, size_t idx) const override;
    Vector2 getUV(const Vector3 &, size_t) const override;
    virtual void fetchData(const Ray &ray, IntersectionData &isec) const override;
    AABB getAABB() const override;

protected:
    std::shared_ptr<Shape> bvh;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<std::shared_ptr<ShapeNormalUV>>    faces;

    friend class MeshTriangle;
    friend class MeshQuad;
};

class GMesh: public Instance
{
public:
    GMesh(std::shared_ptr<Mesh> mesh);

    void fetchData(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};

class MeshFace: public ShapeNormalUV
{
public:
    size_t idx;
    MeshFace(): idx(0){}
    ~MeshFace() {}
};

class MeshQuad: public MeshFace
{
public:
    MeshQuad() = default;
    MeshQuad(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t v3, size_t nv0, size_t nv1, size_t nv2, size_t nv3);

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;

    AABB getAABB() const override;
    Vector3 getNormal(const Vector3 &, size_t ) const override;

    friend std::ostream& operator << (std::ostream &os, const MeshQuad &q);

private:
    const Mesh *mesh;
    size_t v[4];
    size_t nv[4];
    Vector3 nf;
    float area;
    AABB aabb;

    //ignore those for now
    Vector2 getUV(const Vector3 &, size_t) const override;
    void fetchData(const Ray &, IntersectionData &) const override;
};

class MeshTriangle: public MeshFace
{
public:
    MeshTriangle() = default;
    MeshTriangle(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2 );

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;

    AABB getAABB() const override;
    Vector3 getNormal(const Vector3 &, size_t ) const override;

    friend std::ostream& operator << (std::ostream &os, const MeshTriangle &t);

private:
    //pointer to the mesh
    const Mesh *mesh;

    size_t v[3];    // vertices index
    size_t nv[3];   // normals index

    Vector3 nf;

    float area;

    AABB aabb;

    //ignore those for now
    Vector2 getUV(const Vector3 &, size_t) const override;
    void fetchData(const Ray &, IntersectionData &) const override;
};


