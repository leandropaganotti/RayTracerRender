#include "mesh.h"
#include <float.h>

Mesh::Mesh(const Vector3f &color):
	Object(color)
{

}

void Mesh::addVertex(const Vector3f &v)
{
    vertices.push_back(v);
}

void Mesh::addNormal(const Vector3f &v)
{
    normals.push_back(v);
}

void Mesh::addFace(size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2)
{
    Triangle tri;

    size_t vbs = vertices.size();
    if(v0 >= vbs || v1 >= vbs || v2 >= vbs)
        throw std::out_of_range("Vertex index is out of Range");

    size_t nbs = normals.size();
    if(nv0 >= nbs || nv1 >= nbs || nv2 >= nbs)
        throw std::out_of_range("Normal index is out of Range");

    tri.v0 = v0;
    tri.v1 = v1;
    tri.v2 = v2;
    tri.nv0 = nv0;
    tri.nv1 = nv1;
    tri.nv2 = nv2;
    tri.nf = (normals[nv0] + normals[nv1] + normals[nv2]).normalize();
    tri.area = ((vertices[v1] - vertices[v0]) % (vertices[v2] - vertices[v0])).length() / 2;
    faces.push_back(tri);
}

void Mesh::clear()
{
    vertices.clear();
    normals.clear();
    faces.clear();
}

bool Mesh::intersection(const Ray& ray, IntersectionData &isec) const
{
    float tnear;

    if (!aabb.intersection(ray, tnear))
        return false;

    isec.tnear = FLT_MAX;
    for (size_t i=0 ; i < faces.size(); ++i)
    {
        if (faces[i].intersection(vertices, ray, tnear))
        {
            if (tnear < isec.tnear)
            {
                isec.tnear = tnear;
                isec.idx = i;
            }
        }
    }
    if (isec.tnear < FLT_MAX)
    {
        isec.object = this;
        return true;
    }
    return false;
}

bool Mesh::intersection(const Ray& ray, float &tnear) const
{    
    if (!aabb.intersection(ray, tnear))
        return false;

    float t;
    tnear = FLT_MAX;
    for (size_t i=0 ; i < faces.size(); ++i)
    {
        if (faces[i].intersection(vertices, ray, t))
        {
            if (t < tnear) tnear = t;
        }
    }
    return tnear < FLT_MAX ? true : false;
}

const Vector3f Mesh::normal(const Vector3f &phit, size_t idx) const
{    
    float u = (((vertices[faces[idx].v2] - vertices[faces[idx].v1]) % (phit - vertices[faces[idx].v1])).length() / 2) / faces[idx].area;
    float v = (((vertices[faces[idx].v0] - vertices[faces[idx].v2]) % (phit - vertices[faces[idx].v2])).length() / 2) / faces[idx].area;
    float w = 1 - u - v;

    return u*normals[faces[idx].nv0] + v*normals[faces[idx].nv1] +w*normals[faces[idx].nv2];
}

void Mesh::computeAABB()
{
    float minX=FLT_MAX, maxX=FLT_MIN;
    float minY=FLT_MAX, maxY=FLT_MIN;
    float minZ=FLT_MAX, maxZ=FLT_MIN;

    for(size_t i= 0 ; i < vertices.size(); ++i)
    {
        if (vertices[i].x < minX)
            minX = vertices[i].x;
        if (vertices[i].y < minY)
            minY = vertices[i].y;
        if (vertices[i].z < minZ)
            minZ = vertices[i].z;

        if (vertices[i].x > maxX)
            maxX = vertices[i].x;
        if (vertices[i].y > maxY)
            maxY = vertices[i].y;
        if (vertices[i].z > maxZ)
            maxZ = vertices[i].z;
    }
    Vector3f translate, scale;
    
    scale.x = maxX - minX;
    scale.y = maxY - minY;
    scale.z = maxZ - minZ;

    translate.x = minX + (maxX - minX) / 2.0f;
    translate.y = minY + (maxY - minY) / 2.0f;
    translate.z = minZ + (maxZ - minZ) / 2.0f;

    aabb.setTransformation(translate, {}, scale);
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

std::ostream& operator <<(std::ostream &os, const Mesh::Triangle &f)
{
    return os << f.v0 << " " << f.v1 << " " << f.v2 << " - " << "  " << f.nf ;
}

inline
bool Mesh::Triangle::intersection(const std::vector<Vector3f> &vertices, const Ray &ray, float &tnear) const
{
    const Vector3f &p0 = vertices[ v0 ];
    const Vector3f &p1 = vertices[ v1 ];
    const Vector3f &p2 = vertices[ v2 ];

    float A = p0.x - p1.x;
    float B = p0.y - p1.y;
    float C = p0.z - p1.z;

    float D = p0.x - p2.x;
    float E = p0.y - p2.y;
    float F = p0.z - p2.z;

    float G = ray.direction.x;
    float H = ray.direction.y;
    float I = ray.direction.z;

    float J = p0.x - ray.origin.x;
    float K = p0.y - ray.origin.y;
    float L = p0.z - ray.origin.z;


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

    tnear = -(F*AKJB + E*JCAL + D*BLKC  ) / denon;

    return tnear > 0.0f ? true : false;
}
