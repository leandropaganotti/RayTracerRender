#include "mesh.h"
#include <float.h>

std::vector<Vertex>   Mesh::vertexBuffer;

std::vector<Vertex>   Mesh::normalBuffer;

Mesh::Mesh(const Vector3f &color):
	Object(color)
{

}

Face::Face(size_t v0, size_t v1, size_t v2, size_t nv0, size_t nv1, size_t nv2): v0(v0), v1(v1), v2(v2), nv0(nv0), nv1(nv1), nv2(nv2)
{
    size_t vbs = Mesh::vertexBuffer.size();
    if(v0 >= vbs || v1 >= vbs || v2 >= vbs)
        throw std::out_of_range("Vertex index is out of Range");

    size_t nbs = Mesh::normalBuffer.size();
    if(nv0 >= nbs || nv1 >= nbs || nv2 >= nbs)
        throw std::out_of_range("Normal index is out of Range");

    nf = (Mesh::normalBuffer[nv0] + Mesh::normalBuffer[nv1] + Mesh::normalBuffer[nv2]).normalize();


    area = ((Mesh::vertexBuffer[v1] - Mesh::vertexBuffer[v0]) % (Mesh::vertexBuffer[v2] - Mesh::vertexBuffer[v0])).length() / 2;
}

size_t Mesh::addVertex(const Vertex &v)
{
    vertexBuffer.push_back(v);
    size_t idx = vertexBuffer.size() - 1;
    vertices.push_back(idx);
    return idx;
}

size_t Mesh::addNormal(const Vertex &v)
{
    normalBuffer.push_back(v);
    return normalBuffer.size() - 1;
}

size_t Mesh::addFace(const Face &t)
{
    faces.push_back(t);
    return faces.size() - 1;
}

bool Mesh::intersection(const Ray& ray, IntersectionData &isec) const
{   
    float tnear;
    isec.tnear = FLT_MAX;
    for (size_t i=0 ; i < faces.size(); ++i)
    {
        if (faces[i].intersection(ray, tnear))
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
    float t;
    tnear = FLT_MAX;
    for (size_t i=0 ; i < faces.size(); ++i)
    {
        if (faces[i].intersection(ray, t))
        {
            if (t < tnear) tnear = t;
        }
    }
    return tnear < FLT_MAX ? true : false;
}

const Vector3f Mesh::normal(const Vector3f &phit, size_t idx) const
{    
    float u = (((Mesh::vertexBuffer[faces[idx].v2] - Mesh::vertexBuffer[faces[idx].v1]) % (phit - Mesh::vertexBuffer[faces[idx].v1])).length() / 2) / faces[idx].area;
    float v = (((Mesh::vertexBuffer[faces[idx].v0] - Mesh::vertexBuffer[faces[idx].v2]) % (phit - Mesh::vertexBuffer[faces[idx].v2])).length() / 2) / faces[idx].area;
    float w = 1 - u - v;

    return u*Mesh::normalBuffer[faces[idx].nv0] + v*Mesh::normalBuffer[faces[idx].nv1] +w*Mesh::normalBuffer[faces[idx].nv2];

    //return faces[idx].nf;
}

std::ostream &operator <<(std::ostream &os, const Mesh &m)
{
    os << "Mesh:" << std::endl;
    os << "|Vertices: " << std::endl;
    for(size_t i= 0 ; i < m.vertices.size(); ++i)
        std::cout << "||" << i << ":" << m.vertices[i] << " ---> " << m.vertexBuffer[m.vertices[i]] << std::endl;

    os << "|Faces: " << std::endl;
    for(size_t i= 0 ; i < m.faces.size(); ++i)
        std::cout << "||" << i << ":" << m.faces[i] << std::endl;

    return os ;
}

std::ostream& operator <<(std::ostream &os, const Face &f)
{
    return os << f.v0 << " " << f.v1 << " " << f.v2 << " ---> " << Mesh::vertexBuffer[f.v0] << Mesh::vertexBuffer[f.v1] << Mesh::vertexBuffer[f.v2] << "  " << f.nf ;
}

inline
bool Triangle::intersection(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Ray &ray, float &tnear)
{
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

inline
bool Face::intersection(const Ray &ray, float &tnear) const
{
    const Vector3f *p0 = &Mesh::vertexBuffer[ v0 ];
    const Vector3f *p1 = &Mesh::vertexBuffer[ v1 ];
    const Vector3f *p2 = &Mesh::vertexBuffer[ v2 ];

    return Triangle::intersection(*p0, *p1, *p2, ray, tnear);
}
