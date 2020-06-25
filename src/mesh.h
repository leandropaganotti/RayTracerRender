#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include "object.h"

class MeshTriangle;

class Mesh: public GroupedObject
{    
public:
    static std::shared_ptr<Mesh> Create();

    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(const std::shared_ptr<MeshTriangle> &face);

    void clear();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);

protected:
    Mesh();
    std::vector<Vector3>                       vertices;
    std::vector<Vector3>                       normals;
    std::shared_ptr<Material>                  material;

    friend class MeshTriangle;
};




