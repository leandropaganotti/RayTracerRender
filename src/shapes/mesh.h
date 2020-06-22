#pragma once

#include <vector>
#include "shape.h"
#include "instance.h"
#include "aabb.h"
#include "resource.h"

 class BVH;
 class MeshFace;

class Mesh: public Shape, public Resource
{    
public:
    static std::shared_ptr<Mesh> Create(const std::string &key);

    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(std::shared_ptr<MeshFace> face);

    void clear();
    void buildBoundingVolume();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;
    void getNormal(IntersectionData& isec) const override;
    Vector2 getUV(const Vector3 &, size_t) const override;
    AABB getAABB() const override;

protected:
    Mesh();
    std::shared_ptr<IntersectionIF> bvh;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<std::shared_ptr<Shape>>    faces;

    friend class MeshTriangle;
    friend class MeshQuad;
};

class GMesh: public Instance
{
public:
    GMesh(std::shared_ptr<Mesh> mesh);

    void getIsecData(const Ray &ray, IntersectionData &isec) const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material * getMaterial(size_t) const override;

protected:
    std::shared_ptr<Material> material;
};

class MeshFace: public Shape
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
    void getNormal(IntersectionData& isec) const override;

    friend std::ostream& operator << (std::ostream &os, const MeshQuad &q);

private:
    const Mesh *mesh;
    size_t v[4];
    size_t nv[4];
    Vector3 nf;
    float area;
    AABB aabb;
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
    void getNormal(IntersectionData& isec) const override;

    friend std::ostream& operator << (std::ostream &os, const MeshTriangle &t);

private:
    //pointer to the mesh
    const Mesh *mesh;

    size_t v[3];    // vertices index
    size_t nv[3];   // normals index

    Vector3 nf;

    float area;

    AABB aabb;
};


