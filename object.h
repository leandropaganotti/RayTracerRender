#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include "vector.h"
#include "ray.h"

class Object;

typedef std::vector<Object*> ObjectVector;

struct IntersectionData
{
    float dist;
    Vector3f normal;
    Vector3f phit;
    const Object * object;
};

class Object
{   
public:
    Object(const Vector3f& color={}): c_diffuse{color}
    {
        k_ambient = 0.05f;
        k_diffuse = 1.0f;
        k_specular = 0.5f;
        c_specular = {1.0f};
        shininess = 30.0f;
    }

    virtual bool intersection(const Ray& ray, IntersectionData &inter) const = 0;
    virtual bool intersection(const Ray& ray, float &dist) const = 0;

    virtual ~Object(){}

    //Object attributes down here

    Vector3f c_diffuse;
    Vector3f c_specular;

    float k_diffuse;
    float k_specular;
    float k_ambient;

    float shininess;
};

#endif // OBJECT_H
