#include "mesh.h"
#include <float.h>



bool Mesh::triangleIntersection(size_t idx, const Ray &ray, float &tnear) const
{
    const Vector3f &p0 = static_cast< const Vector3f &> (vertices[ triangles[idx].v[0] ]);
    const Vector3f &p1 = static_cast< const Vector3f &> (vertices[ triangles[idx].v[1] ]);
    const Vector3f &p2 = static_cast< const Vector3f &> (vertices[ triangles[idx].v[2] ]);

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

size_t Mesh::addVertex(const Vertex &v)
{
    vertices.push_back(v);
    return vertices.size() - 1;
}

size_t Mesh::addTriangle(const Triangle &tri)
{
    triangles.push_back(tri);
    return triangles.size() - 1;
}


bool Mesh::intersection(const Ray& ray, IntersectionData &isec) const
{   
    float tnear;
    isec.tnear = FLT_MAX;
    size_t tri;
    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangleIntersection(i, ray, tnear))
        {
            if (tnear < isec.tnear)
            {
                isec.tnear = tnear;
                tri = i;
            }
        }
    }
    if (isec.tnear < FLT_MAX)
    {
        isec.normal = triangles[tri].normal;
        isec.object = this;
        isec.phit   = ray.origin + isec.tnear * ray.direction;
        return true;
    }
    return false;
}

bool Mesh::intersection(const Ray& ray, float &tnear) const
{
    float t;
    tnear = FLT_MAX;
    for (size_t i=0 ; i < triangles.size(); ++i)
    {
        if (triangleIntersection(i, ray, t))
        {
            if (t < tnear) tnear = t;
        }
    }
    return tnear < FLT_MAX ? true : false;
}

std::ostream &operator <<(std::ostream &os, const Mesh &m)
{
    os << "Mesh:" << std::endl;
    os << "|Vertices: " << std::endl;
    for(size_t i= 0 ; i < m.vertices.size(); ++i)
        std::cout << "||" << i << "->" << m.vertices[i] << std::endl;

    os << "|Triangles: " << std::endl;
    for(size_t i= 0 ; i < m.triangles.size(); ++i)
        std::cout << "||" << i << "->" << m.triangles[i] << std::endl;

    return os ;
}

Triangle::Triangle(size_t v0, size_t v1, size_t v2, const Vector3f &n):
    v{v0, v1, v2}, normal(n)
{
//    size_t vbs = vertexBuffer.size();
//    if(v0 >= vbs || v1 >= vbs || v2 >= vbs)
//        throw std::out_of_range("Vertex index is out of Range");

//    area = ((vertexBuffer[v1]-vertexBuffer[v0]) % (vertexBuffer[v2]-vertexBuffer[v0])).length() / 2.0f;
}

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



//bool Triangle::intersection(const Ray &ray, Vector3f &phit, float & tnear) const
//{
//    if (Plane::intersection(vertexBuffer[v[0]], normal, ray, tnear))
//    {
//        phit = ray.origin + tnear * ray.direction;
//        return isInside2(phit);
//    }
//    return false;
//}

//bool Triangle::intersection(const Ray &ray, float &tnear) const
//{
//    Vector3f &p0 = vertexBuffer[v[0]];
//    Vector3f &p1 = vertexBuffer[v[1]];
//    Vector3f &p2 = vertexBuffer[v[2]];

//    float A = p0.x - p1.x;
//    float B = p0.y - p1.y;
//    float C = p0.z - p1.z;

//    float D = p0.x - p2.x;
//    float E = p0.y - p2.y;
//    float F = p0.z - p2.z;

//    float G = ray.direction.x;
//    float H = ray.direction.y;
//    float I = ray.direction.z;

//    float J = p0.x - ray.origin.x;
//    float K = p0.y - ray.origin.y;
//    float L = p0.z - ray.origin.z;


//    float EIHF = E*I-H*F;
//    float GFDI = G*F-D*I;
//    float DHEG = D*H-E*G;

//    float denon = A*EIHF + B*GFDI + C*DHEG;

//    float beta = (J*EIHF + K*GFDI + L*DHEG) / denon;

//    if (beta <= 0.0f || beta >= 1.0f) return false;

//    float AKJB = A*K-J*B;
//    float JCAL = J*C-A*L;
//    float BLKC = B*L-K*C;

//    float gamma = (I*AKJB + H*JCAL + G*BLKC ) / denon;

//    if (gamma <= 0.0f || beta + gamma >= 1.0f) return false;

//    tnear = -(F*AKJB + E*JCAL + D*BLKC  ) / denon;

//    return tnear > 0.0f ? true : false;
//}

std::ostream &operator <<(std::ostream &os, const Triangle &t)
{
    return os << t.v[0] << " " << t.v[1] << " " << t.v[2] << " " << t.normal << " " << t.area;
}

//bool Triangle::isInside2(const Vector3f &P) const // area
//{
//    Vector3f PA = vertexBuffer[v[0]]-P;
//    Vector3f PB = vertexBuffer[v[1]]-P;
//    Vector3f PC = vertexBuffer[v[2]]-P;
//    float a1 = ((PA) % (PB)).length()/2.0f ;// PAB
//    float a2 = ((PA) % (PC)).length()/2.0f ;// PAC
//    float a3 = ((PB) % (PC)).length()/2.0f ;// PBC
//    return fabs(area-a1-a2-a3) < 0.0001f;
//}

//bool Triangle::isInside(const Vector3f &P) const
//{
//    Vector3f e0 = vertexBuffer[v[1]] - vertexBuffer[v[0]];
//    Vector3f e1 = vertexBuffer[v[2]] - vertexBuffer[v[1]];
//    Vector3f e2 = vertexBuffer[v[0]] - vertexBuffer[v[2]];
//    Vector3f C0 = P - vertexBuffer[v[0]];
//    Vector3f C1 = P - vertexBuffer[v[1]];
//    Vector3f C2 = P - vertexBuffer[v[2]];
//    return ((normal ^ (e0 % C0)) > 0 && (normal ^ (e1 % C1)) > 0 && (normal ^ (e2 % C2)) > 0); // P is inside the triangle
//}

//std::ostream &operator <<(std::ostream &os, const VertexBuffer &vb)
//{
//    os << "VertexBuffer: " << std::endl;
//    for(size_t i= 0 ; i < vb.size(); ++i)
//        os << "|" << i << "->" << vb[i] << std::endl;
//    return os;
//}


