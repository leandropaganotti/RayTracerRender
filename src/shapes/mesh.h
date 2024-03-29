#pragma once

#include <vector>
#include "primitive.h"
#include "aabb.h"
#include "object.h"
#include "aggregate.h"

class MeshTriangle;

class Mesh: public Shape
{    
public:
    Mesh();
    Mesh(const std::shared_ptr<Aggregate<MeshTriangle>> &agg);
    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(const std::shared_ptr<MeshTriangle> &face);
    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

    void buildAggregate();

protected:
    std::vector<Vector3>                        vertices;
    std::vector<Vector3>                        normals;
    std::shared_ptr<Aggregate<MeshTriangle>>    faces;

    friend class MeshTriangle;

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        return faces->intersection(ray, isec);
    }
    bool intersection(const Ray &ray) const override
    {
        return faces->intersection(ray);
    }
    AABB getAABB() const override
    {
        return faces->getAABB();
    }
};




