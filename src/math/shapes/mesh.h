#pragma once

#include <vector>
#include "shape.h"
#include "aabb.h"
#include <map>

struct BVH;

class Mesh: public Shape
{    
public:
    Mesh() = default;

    struct Triangle
    {
        size_t v0, v1, v2;      // 3 vertex indexes
        size_t nv0, nv1, nv2;   // 3 normal indexes for vertices
        Vector3 nf;             // face normal
        float area;
        bool intersection(const std::vector<Vector3>   &vertices, const Ray &ray, float tmax, float &tnear) const;
    };

    void addVertex(const Vector3& v);
    void addNormal(const Vector3& n);
    void addFace(size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2);

    void clear();
    void updateAABB();

    friend std::ostream& operator << (std::ostream& os, const Mesh &m);
    friend std::ostream& operator << (std::ostream &os, const Triangle &f);

    // Shape interface
    bool  intersection(const Ray& ray, float tmax, IntersectionData& isec) const;
    bool  intersection(const Ray& ray, float tmax) const;
    Vector3 normal(const Vector3 &phit, size_t idx) const;
    Vector2 uv(const Vector3 &, size_t) const;
    virtual void fetch(const Ray &ray, IntersectionData &isec) const override;

protected:
    std::unique_ptr<BVH>   aabb;
    std::vector<Vector3>   vertices;
    std::vector<Vector3>   normals;
    std::vector<Triangle>  faces;    
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
    std::vector<Mesh::Triangle> &t;
    Node *root;
    std::map<Node*,     size_t> m;

    Node* build(size_t l, size_t r, size_t axis);
    bool intersection(Node* root, const Ray &ray, float tmax, IntersectionData &isec);
    bool intersection(Node* root, const Ray &ray, float tmax);
    size_t qsplit(size_t l, size_t r, float pivot, size_t axis);

public:
    BVH(std::vector<Vector3> &v, std::vector<Mesh::Triangle> &t);
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
