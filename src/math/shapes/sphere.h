#pragma once

#include "shape.h"
#include "utils.h"

class Sphere: public Shape
{    
public:        
    bool  intersection(const Ray &ray, IntersectionData &isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    Vector3 normal(const Vector3 &phit, size_t) const;
    std::pair<float, float> uv(const Vector3 &phit, size_t) const;

    Vector3  getCenter() const;
    void     setCenter(const Vector3 &value);
    float    getRadius() const;
    void     setRadius(float value);    

    void sampleSolidAngleSphere(const Vector3& point, Vector3& sample,  float &_1_pdf) const;

protected:
    Sphere(const Vector3 &center={0.0f}, const float &radius=1.0f);

    Vector3 center;                         // position of the sphere
    float radius, radius2;                  // sphere radius and radius^2

    friend class Shapes;
};

inline
void Sphere::sampleSolidAngleSphere(const Vector3& point, Vector3& sample,  float &_1_pdf) const{
    float dist2 = (center - point).length(); dist2*=dist2; // distance from point hit to center of light power 2
    float cos_a_max = sqrt(1.0f - radius2/dist2);
    float r1 = dis(gen);
    float r2 = dis(gen);
    float cos_a = 1.0f + r1*(cos_a_max-1.0f);
    float phi = 2.0f * M_PI * r2;
    //float sin_a = sqrtf(1-cos_a*cos_a);
    float sin_a = sinf(acosf(cos_a));
    Vector3 u,v, w=(center - point).normalize(), n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;
    sample = u*(cos(phi)*sin_a) + v*(sin(phi)*sin_a) + w*(cos_a);

    _1_pdf = (2.0f*M_PI*(1.0f-cos_a_max));//1/(2*M_PI*(1-cos_a_max));
}

