#include "mesh.h"
#include <float.h>
#include "material.h"
#include "invisible.h"
#include "bvh.h"

/************************************************************************
 * Mesh class
 ************************************************************************/

std::shared_ptr<Mesh> Mesh::Create()
{
    return std::shared_ptr<Mesh>(new Mesh);
}

void Mesh::addVertex(const Vector3 &v)
{
    vertices.push_back(v);
}

void Mesh::addNormal(const Vector3 &n)
{
    normals.push_back(n);
}

void Mesh::addFace(const std::shared_ptr<MeshTriangle> &face)
{
    if(face)
    {
        face->idx = faces.size();
        auto obj = std::shared_ptr<SimpleObject>(new SimpleObject(face, material));
        faces.push_back(std::move(obj));
    }
}

void Mesh::clear()
{
    vertices.clear();
    normals.clear();
    faces.clear();
}

void Mesh::buildBoundingVolume()
{
    bvh = BVH::Create(faces);
}

bool Mesh::intersection(const Ray& ray, IntersectionData &isec) const
{
    Ray r = ray;
    r.invdir = 1.0f / ray.direction;
    r.posneg[0] = r.direction[0] > 0 ? 0 : 1;
    r.posneg[1] = r.direction[1] > 0 ? 0 : 1;
    r.posneg[2] = r.direction[2] > 0 ? 0 : 1;
    return bvh->intersection(r, isec);
}

bool Mesh::intersection(const Ray& ray) const
{
    Ray r = ray;
    r.invdir = 1.0f / ray.direction;
    r.posneg[0] = r.direction[0] > 0 ? 0 : 1;
    r.posneg[1] = r.direction[1] > 0 ? 0 : 1;
    r.posneg[2] = r.direction[2] > 0 ? 0 : 1;
    return bvh->intersection(r);
}

void Mesh::getIsecData(IntersectionData &isec) const
{
    faces[isec.idx]->getIsecData(isec);
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

AABB Mesh::getAABB() const
{
    return bvh->getAABB();
}

Mesh::Mesh()
{

}

void Mesh::setMaterial(const std::shared_ptr<Material> &value)
{
    for (auto &f: faces)
        f->setMaterial(value);
}

/************************************************************************
 * TriangleMesh class
 ************************************************************************/

MeshTriangle::MeshTriangle(const Mesh *m, size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2)
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

bool MeshTriangle::intersection(const Ray &ray, IntersectionData &isec) const
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

    if(tval < 0.0f || tval > ray.tmax) return false;

    isec.tnear = tval;
    isec.idx = idx;
    isec.shape = this;
    return true;
}

bool MeshTriangle::intersection(const Ray &ray) const
{
    IntersectionData isec;
    return intersection(ray, isec);
}

void MeshTriangle::getIsecData(IntersectionData &isec) const
{
    getUV(isec);
    getNormal(isec);
}

AABB MeshTriangle::getAABB() const
{
    return aabb;
}

inline
void MeshTriangle::getNormal(IntersectionData& isec) const
{
    float _u = (((mesh->vertices[v[2]] - mesh->vertices[v[1]]) % (isec.phit - mesh->vertices[v[1]])).length() / 2) / area;
    float _v = (((mesh->vertices[v[0]] - mesh->vertices[v[2]]) % (isec.phit - mesh->vertices[v[2]])).length() / 2) / area;
    float _w = 1 - _u - _v;

    isec.normal = _u*mesh->normals[nv[0]] + _v*mesh->normals[nv[1]] + _w*mesh->normals[nv[2]];

    //isec.normal = (mesh->normals[nv[0]] + mesh->normals[nv[1]] + mesh->normals[nv[2]]).normalize();
}

inline
void MeshTriangle::getUV(IntersectionData &isec) const
{
    isec.uv = Vector2(0);
}

std::ostream& operator <<(std::ostream &os, const MeshTriangle &t)
{
    return os << t.v[0] << " " << t.v[1] << " " << t.v[2] << " - " << "  " << t.nf ;
}
