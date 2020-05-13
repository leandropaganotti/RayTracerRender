#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include <map>

 class BVH;
 class Mesh;

class QuadMesh: public Shape
{   
    size_t v[4];
    size_t nv[4];
    Vector3 nf;
    const Vector3 *verts;

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float tmax) const;

    //ignore those
    Vector3 normal(const Vector3 &, size_t ) const;
    Vector2 uv(const Vector3 &, size_t) const;
    void fetch(const Ray &, IntersectionData &) const;
};

class TriangleMesh: public Shape
{
public:
    TriangleMesh() = default;
    TriangleMesh(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2 );

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float tmax) const;

    size_t v[3];    // vertices index
    size_t nv[3];   // normals index

    Vector3 nf;

    float area;

    friend std::ostream& operator << (std::ostream &os, const TriangleMesh &t);

private:
    //pointer to the mesh
    const Mesh *mesh;

    //ignore those
    Vector3 normal(const Vector3 &, size_t ) const;
    Vector2 uv(const Vector3 &, size_t) const;
    void fetch(const Ray &, IntersectionData &) const;
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
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float tmax) const;
    Vector3 normal(const Vector3 &phit, size_t idx) const;
    Vector2 uv(const Vector3 &, size_t) const;
    virtual void fetch(const Ray &ray, IntersectionData &isec) const override;

protected:
    std::unique_ptr<BVH>   bvh;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<TriangleMesh>  faces;

    friend class TriangleMesh;
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


class BVH
{
    struct Node: public AABB
    {
        Node *left=nullptr;
        Node *right=nullptr;
        ~Node()
        {
            if(left) delete left;
            if(right) delete right;
        }
    };

    std::vector<Vector3> &v;
    std::vector<TriangleMesh> &t;
    Node *root;
    std::map<Node*,     size_t> m;

    Node* build(size_t l, size_t r, size_t axis);
    bool intersection(Node* root, const Ray &ray, float tmax, IntersectionData &isec);
    bool intersection(Node* root, const Ray &ray, float tmax);
    size_t qsplit(size_t l, size_t r, float pivot, size_t axis);

public:
    BVH(std::vector<Vector3> &v, std::vector<TriangleMesh> &t);
    ~BVH();

    bool intersection(const Ray &ray, float tmax)
    {
        return intersection(root, ray, tmax);
    }
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec)
    {
        return intersection(root, ray, tmax, isec);
    }
};
