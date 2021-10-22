#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include "object.h"
#include "aggregate.h"

class MeshTriangle;

class Mesh: public Intersection
{    
public:
    Mesh();
    Mesh(const std::shared_ptr<Aggregate> &agg);
    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(const std::shared_ptr<MeshTriangle> &face);
    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

    void buildAggregate();

protected:
    std::vector<Vector3>                        vertices;
    std::vector<Vector3>                        normals;
    std::vector<std::shared_ptr<Shape>>         faces;
    std::shared_ptr<Aggregate>                  aggregate;

    friend class MeshTriangle;

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        return aggregate->intersection(ray, isec);
    }
    bool intersection(const Ray &ray) const override
    {
        return aggregate->intersection(ray);
    }
    AABB getAABB() const override
    {
        return aggregate->getAABB();
    }
};




