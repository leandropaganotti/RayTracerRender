#include "mesh.h"
#include <float.h>
#include "objparser.h"
#include "material.h"

/************************************************************************
 * Mesh class
 ************************************************************************/

void Mesh::addVertex(const Vector3 &v)
{
    vertices.push_back(v);
}

void Mesh::addNormal(const Vector3 &n)
{
    normals.push_back(n);
}

void Mesh::addFace(const TriangleMesh &f)
{
    faces.push_back(f);
}

void Mesh::clear()
{
    vertices.clear();
    normals.clear();
    faces.clear();
}

void Mesh::updateAABB()
{
    bvh.reset(new BVH(vertices, faces));
}

bool Mesh::intersection(const Ray& ray, float tmax, IntersectionData &isec) const
{
    return bvh->intersection(ray, tmax, isec);
}

bool Mesh::intersection(const Ray& ray, float tmax) const
{
    return bvh->intersection(ray, tmax);
}

std::ostream &operator <<(std::ostream &os, const Mesh &m)
{
    os << "Mesh:" << std::endl;
    os << "|Vertices: " << std::endl;
    for(size_t i= 0 ; i < m.vertices.size(); ++i)
        std::cout << "||" << i << ":" << m.vertices[i] << " ---> " << m.vertices[i] << std::endl;

    os << "|Faces: " << std::endl;
    for(size_t i= 0 ; i < m.faces.size(); ++i)
        std::cout << "||" << i << ":" << m.faces[i] << std::endl;

    return os ;
}


Vector3 Mesh::normal(const Vector3 &phit, size_t idx) const
{
    float u = (((vertices[faces[idx].v[2]] - vertices[faces[idx].v[1]]) % (phit - vertices[faces[idx].v[1]])).length() / 2) / faces[idx].area;
    float v = (((vertices[faces[idx].v[0]] - vertices[faces[idx].v[2]]) % (phit - vertices[faces[idx].v[2]])).length() / 2) / faces[idx].area;
    float w = 1 - u - v;

    Vector3 N =  u*normals[faces[idx].nv[0]] + v*normals[faces[idx].nv[1]] +w*normals[faces[idx].nv[2]];

    //N = (normals[faces[idx].nv0] + normals[faces[idx].nv1] + normals[faces[idx].nv2]).normalize();

    return N;
}


Vector2 Mesh::uv(const Vector3 &, size_t) const
{
    return Vector2(0.0f, 0.0f);
}


void Mesh::fetch(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = normal(isec.phit, isec.idx);
}

/************************************************************************
 * GMesh class
 ************************************************************************/

GMesh::GMesh(std::shared_ptr<Mesh> mesh): Instance(mesh)
{
    material = Material::DiffuseWhite;
}

void GMesh::fetch(const Ray &ray, IntersectionData &isec) const
{
    Instance::fetch(ray, isec);
    isec.material = material.get();
    isec.color = material->Kd;
}

std::shared_ptr<Material> GMesh::getMaterial() const
{
    return material;
}

void GMesh::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : Material::DiffuseWhite;
}

/************************************************************************
 * BVH class
 ************************************************************************/

BVH::BVH(std::vector<Vector3> &v, std::vector<TriangleMesh> &t): v(v), t(t)
{
    root = build(1, t.size()-1, 0);
}


BVH::~BVH(){ delete root; }


BVH::Node* BVH::build(size_t l, size_t r, size_t axis)
{
    Node *bvh = new Node;

    for (size_t i = l; i <= r; ++i)
    {
        bvh->extend(v[t[i].v[0]]);
        bvh->extend(v[t[i].v[1]]);
        bvh->extend(v[t[i].v[2]]);
    }

    if (l==r)
    {
        m[bvh] = l;
        return bvh;
    }
    if(l == (r-1))
    {
        bvh->left = new Node;
        bvh->right = new Node;
        m[bvh->left] = l;
        m[bvh->right] = r;
        return bvh;
    }

    size_t pivot = qsplit(l, r, bvh->getCenter()[axis], axis);
    bvh->left = build(l, pivot-1, (axis+1)%3);
    bvh->right = build(pivot, r, (axis+1)%3);

    return bvh;
}

bool BVH::intersection(Node *root, const Ray &ray, float tmax, IntersectionData &isec)
{
    if(!root->left)
    {
        if(t[ m[root] ].intersection(ray, tmax, isec))
        {
            isec.idx = m[root];
            return true;
        }
        return false;
    }

    if(!root->intersection(ray, tmax)) return false;

    bool hit_left = intersection(root->left, ray, tmax, isec);
    if ( hit_left )
    {
        tmax = isec.tnear;
    }

    bool hit_right = intersection(root->right, ray, tmax, isec);
    return hit_left || hit_right;
}

bool BVH::intersection(Node *root, const Ray &ray, float tmax)
{
    if(!root->left)
        return t[ m[root] ].intersection(ray, tmax);

    if(!root->intersection(ray, tmax)) return false;

    if ( intersection(root->left, ray, tmax) ) return true;
    return intersection(root->right, ray, tmax);
}

size_t BVH::qsplit(size_t l, size_t r, float pivot, size_t axis)
{
    size_t j = l-1;
    for(size_t i=l; i <= r; ++i)
    {
        float center = (v[t[i].v[0]][axis] + v[t[i].v[1]][axis] + v[t[i].v[2]][axis]) / 3;
        if (center < pivot)
        {
            ++j;
            TriangleMesh tmp = t[i];
            t[i] = t[j];
            t[j] = tmp;
        }
    }
    if(j==(l-1) || j==r) return (l+r)/2;
    return j+1;
}

/************************************************************************
 * QuadMesh class
 ************************************************************************/

bool QuadMesh::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    float t = ((verts[v[0]]-ray.origin) ^ nf) / (ray.direction ^ nf);

    if( t < 0.0f || t > tmax) return false;

    isec.tnear = t;
    isec.phit = ray.origin + t * ray.direction;

    return true;
}

bool QuadMesh::intersection(const Ray &ray, float tmax) const
{
    float t = ((verts[v[0]]-ray.origin) ^ nf) / (ray.direction ^ nf);

    if( t < 0.0f || t > tmax) return false;

    return true;
}

Vector3 QuadMesh::normal(const Vector3 &, size_t) const { return Vector3(0);}

Vector2 QuadMesh::uv(const Vector3 &, size_t) const { return Vector2(0);}

void QuadMesh::fetch(const Ray &, IntersectionData &) const {}

/************************************************************************
 * TriangleMesh class
 ************************************************************************/

TriangleMesh::TriangleMesh(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2)
{
    mesh = m;
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    nv[0] = nv0;
    nv[1] = nv1;
    nv[2] = nv2;
    nf = (mesh->normals[nv0] + mesh->normals[nv1] + mesh->normals[nv2]).normalize();
    area = ((mesh->vertices[v1] - mesh->vertices[v0]) % (mesh->vertices[v2] - mesh->vertices[v0])).length() / 2;
}

bool TriangleMesh::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    float A = mesh->vertices[v[0]].x - mesh->vertices[v[1]].x;
    float B = mesh->vertices[v[0]].y - mesh->vertices[v[1]].y;
    float C = mesh->vertices[v[0]].z - mesh->vertices[v[1]].z;

    float D = mesh->vertices[v[0]].x - mesh->vertices[v[2]].x;
    float E = mesh->vertices[v[0]].y - mesh->vertices[v[2]].y;
    float F = mesh->vertices[v[0]].z - mesh->vertices[v[2]].z;

    float G = ray.direction.x;
    float H = ray.direction.y;
    float I = ray.direction.z;

    float J = mesh->vertices[v[0]].x - ray.origin.x;
    float K = mesh->vertices[v[0]].y - ray.origin.y;
    float L = mesh->vertices[v[0]].z - ray.origin.z;


    float EIHF = E*I-H*F;
    float GFDI = G*F-D*I;
    float DHEG = D*H-E*G;

    float denon = A*EIHF + B*GFDI + C*DHEG;

    float beta = (J*EIHF + K*GFDI + L*DHEG) / denon;

    if (beta <= 0.0f || beta >= 1.0f) return false;

    float AKJB = A*K-J*B;
    float JCAL = J*C-A*L;
    float BLKC = B*L-K*C;

    float gamma = (I*AKJB + H*JCAL + G*BLKC ) / denon;

    if (gamma <= 0.0f || beta + gamma >= 1.0f) return false;

    float tval = -(F*AKJB + E*JCAL + D*BLKC  ) / denon;

    if(tval < 0.0f || tval > tmax) return false;

    isec.tnear = tval;
    return true;
}

bool TriangleMesh::intersection(const Ray &ray, float tmax) const
{
    IntersectionData isec;
    return intersection(ray, tmax, isec);
}

Vector3 TriangleMesh::normal(const Vector3 &, size_t) const { return Vector3(0);}

Vector2 TriangleMesh::uv(const Vector3 &, size_t) const { return Vector2(0);}

void TriangleMesh::fetch(const Ray &, IntersectionData &) const {}

std::ostream& operator <<(std::ostream &os, const TriangleMesh &t)
{
    return os << t.v[0] << " " << t.v[1] << " " << t.v[2] << " - " << "  " << t.nf ;
}


