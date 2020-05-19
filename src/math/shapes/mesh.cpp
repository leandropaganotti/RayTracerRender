#include "mesh.h"
#include <float.h>
#include "objparser.h"
#include "material.h"
#include "invisible.h"
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
    faces.back().idx = faces.size()-1;
}

void Mesh::clear()
{
    vertices.clear();
    normals.clear();
    faces.clear();
}

void Mesh::updateAABB()
{
    bvh = BVH::Create(faces, 1, faces.size()-1, 0);
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
        std::cout << "||" << i << ":" << m.vertices[i] << std::endl;

    os << "|Normals: " << std::endl;
    for(size_t i= 0 ; i < m.normals.size(); ++i)
        std::cout << "||" << i << ":" << m.normals[i] << std::endl;

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

BVH::BVH()
{
    left = right = nullptr;
}

BVH::~BVH(){ }


Shape* BVH::Create(std::vector<TriangleMesh> &shapes, size_t l, size_t r, size_t axis)
{
    if (l==r)
    {
        return &shapes[l];
    }

    AABB aabb;
    for (size_t i = l; i <= r; ++i)
    {
        aabb.extend(shapes[i].getAABB());
    }
    BVH *bvh = new BVH;
    bvh->aabb = aabb;
    if(l == (r-1))
    {
        bvh->left = &shapes[l];
        bvh->right = &shapes[r];
        return bvh;
    }
    size_t pivot = qsplit(shapes, l, r, aabb.getCenter()[axis], axis);
    bvh->left = Create(shapes, l, pivot-1, (axis+1)%3);
    bvh->right = Create(shapes, pivot, r, (axis+1)%3);

    return bvh;
}

Vector3 BVH::normal(const Vector3 &phit, size_t idx) const
{

}

Vector2 BVH::uv(const Vector3 &phit, size_t idx) const
{

}

void BVH::fetch(const Ray &ray, IntersectionData &isec) const
{

}

bool BVH::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    if(!aabb.intersection(ray, tmax)) return false;

    bool hit_left = left->intersection(ray, tmax, isec);
    if ( hit_left )
    {
        tmax = isec.tnear;
    }

    bool hit_right = right->intersection(ray, tmax, isec);
    return hit_left || hit_right;
}

bool BVH::intersection(const Ray &ray, float tmax) const
{
    if(!aabb.intersection(ray, tmax)) return false;

    if ( left->intersection(ray, tmax) ) return true;
    return right->intersection(ray, tmax);
}

size_t BVH::qsplit(std::vector<TriangleMesh> &shapes, size_t l, size_t r, float pivot, size_t axis)
{
    size_t j = l-1;
    for(size_t i=l; i <= r; ++i)
    {
        float center = shapes[i].getAABB().getCenter()[axis];
        if (center < pivot)
        {
            ++j;
            TriangleMesh tmp = shapes[i];
            shapes[i] = shapes[j];
            shapes[j] = tmp;

            shapes[i].idx = i;
            shapes[j].idx = j;
        }
    }
    if(j==(l-1) || j==r) return (l+r)/2;
    return j+1;
}

/************************************************************************
 * QuadMesh class
 ************************************************************************/

QuadMesh::QuadMesh(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t v3, size_t nv0, size_t nv1, size_t nv2, size_t nv3)
{
    mesh = m;
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;
    nv[0] = nv0;
    nv[1] = nv1;
    nv[2] = nv2;
    nv[3] = nv3;

    nf = (mesh->normals[nv0] + mesh->normals[nv1] + mesh->normals[nv2]).normalize();
    aabb.extend({mesh->vertices[v0], mesh->vertices[v1], mesh->vertices[v2], mesh->vertices[v3]});
    area = ((mesh->vertices[v1] - mesh->vertices[v0]) % (mesh->vertices[v2] - mesh->vertices[v0])).length() / 2;
    area += ((mesh->vertices[v2] - mesh->vertices[v0]) % (mesh->vertices[v3] - mesh->vertices[v0])).length() / 2;

}

bool QuadMesh::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    float t = ((mesh->vertices[v[0]]-ray.origin) ^ nf) / (ray.direction ^ nf);

    if( t < 0.0f || t > tmax) return false;

    Vector3 phit = ray.origin + t * ray.direction;

    Vector3 PV0 =  mesh->vertices[v[0]] - phit;
    Vector3 PV1 =  mesh->vertices[v[1]] - phit;
    Vector3 PV2 =  mesh->vertices[v[2]] - phit;
    Vector3 PV3 =  mesh->vertices[v[3]] - phit;

    if ((PV0 ^ PV1) > 0.0f &&

    (PV1 ^ PV2) > 0.0f &&

    (PV2 ^ PV3) > 0.0f &&

    (PV3 ^ PV0) > 0.0f) return false;

    isec.tnear = t;
    return true;
}

bool QuadMesh::intersection(const Ray &ray, float tmax) const
{
    float t = ((mesh->vertices[v[0]]-ray.origin) ^ nf) / (ray.direction ^ nf);

    if( t < 0.0f || t > tmax) return false;

    Vector3 phit = ray.origin + t * ray.direction;

    Vector3 PV0 =  mesh->vertices[v[0]] - phit;
    Vector3 PV1 =  mesh->vertices[v[1]] - phit;
    Vector3 PV2 =  mesh->vertices[v[2]] - phit;
    Vector3 PV3 =  mesh->vertices[v[3]] - phit;

    float a  = (PV0 % PV1).length() ;
    a += (PV1 % PV2).length() ;
    a += (PV2 % PV3).length() ;
    a += (PV3 % PV0).length() ;
    a /= 2;
    if((a - area) > 0.001f) return false;

    return true;
}

AABB QuadMesh::getAABB() const
{
    return aabb;
}

std::ostream& operator <<(std::ostream &os, const QuadMesh &q)
{
    return os << q.v[0] << " " << q.v[1] << " " << q.v[2] << " " << q.v[3] << " - " << "  " << q.nf ;
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
    aabb.extend({mesh->vertices[v0], mesh->vertices[v1], mesh->vertices[v2]});
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
    isec.idx = idx;
    return true;
}

bool TriangleMesh::intersection(const Ray &ray, float tmax) const
{
    IntersectionData isec;
    return intersection(ray, tmax, isec);
}

AABB TriangleMesh::getAABB() const
{
    return aabb;
}

Vector3 TriangleMesh::normal(const Vector3 &, size_t) const { return Vector3(0);}

Vector2 TriangleMesh::uv(const Vector3 &, size_t) const { return Vector2(0);}

void TriangleMesh::fetch(const Ray &, IntersectionData &) const {}

std::ostream& operator <<(std::ostream &os, const TriangleMesh &t)
{
    return os << t.v[0] << " " << t.v[1] << " " << t.v[2] << " - " << "  " << t.nf ;
}
