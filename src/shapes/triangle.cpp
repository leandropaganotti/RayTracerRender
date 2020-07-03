#include "triangle.h"
#include "mesh.h"

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
    idx = mesh->objects.size();
}

inline
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
    isec.uv.u = 0;
    isec.uv.v = 0;
}

std::ostream& operator <<(std::ostream &os, const MeshTriangle &t)
{
    return os << t.v[0] << " " << t.v[1] << " " << t.v[2] << " - " << "  " << t.nf ;
}
