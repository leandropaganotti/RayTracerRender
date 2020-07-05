#pragma once

#include "shape.h"
#include "aabb.h"

class Mesh;

class MeshTriangle: public Shape
{
public:
    MeshTriangle(const Mesh *m, size_t v0,  size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2);

    // Shape interface
    bool intersection(const Ray& ray, IntersectionData& isec) const override;
    bool intersection(const Ray& ray) const override;
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
