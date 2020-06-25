#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include "resource.h"
#include "object.h"

 class BVH;
 class MeshTriangle;

class Mesh: public Object
{    
public:
    static std::shared_ptr<Mesh> Create();

    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(const std::shared_ptr<MeshTriangle> &face);

    void clear();
    void buildBoundingVolume();


    bool intersection(const Ray& ray, IntersectionData& isec) const override;
    bool intersection(const Ray& ray) const override;
    void getIsecData(IntersectionData &isec) const override;
    AABB getAABB() const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

protected:
    Mesh();
    std::shared_ptr<IntersectionIF> bvh;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<std::shared_ptr<SimpleObject>>    faces;

    std::shared_ptr<Material> material;

    friend class MeshTriangle;
};

class MeshTriangle: public Shape
{
public:
    MeshTriangle() = default;
    MeshTriangle(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2 );

    // Shape interface
    bool  intersection(const Ray& ray, IntersectionData& isec) const override;
    bool  intersection(const Ray& ray) const override;
    void getIsecData(IntersectionData &isec) const override;
    AABB getAABB() const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &) const override;

    friend std::ostream& operator << (std::ostream &os, const MeshTriangle &t);

protected:
    //pointer to the mesh
    const Mesh *mesh;
    size_t idx;
    size_t v[3];    // vertices index
    size_t nv[3];   // normals index

    Vector3 nf;

    float area;

    AABB aabb;

    friend class Mesh;
};


