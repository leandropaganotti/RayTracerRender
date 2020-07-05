#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include "object.h"
#include "aggregate.h"

class MeshTriangle;

class Mesh: public Object
{    
public:
    Mesh();
    Mesh(const std::shared_ptr<Aggregate> &agg);
    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(const std::shared_ptr<MeshTriangle> &face);
    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

    //set all triagle material
    void setMaterial(const std::shared_ptr<Material> &m) override;

    // set the material to be used when create new triagle
    void useMaterial(const std::shared_ptr<Material> &m);

    void buildAggregate();

protected:
    std::vector<Vector3>                        vertices;
    std::vector<Vector3>                        normals;
    std::vector<std::shared_ptr<Object>>        faces;
    std::shared_ptr<Material>                   mtlInUse; // material in use when creating new faces/triangles
    std::shared_ptr<Aggregate>                  aggregate;

    friend class MeshTriangle;

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        if(aggregate->intersection(ray, isec))
        {
            isec.object = this;
            return true;
        }
        return false;
    }
    bool intersection(const Ray &ray) const override
    {
        return aggregate->intersection(ray);
    }
    AABB getAABB() const override
    {
        return aggregate->getAABB();
    }
    void getIsecData(IntersectionData &isec) const override
    {
        faces[isec.idx]->getIsecData(isec);
    }
};




