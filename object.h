#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include "vector.h"
#include "ray.h"

class Object;

typedef std::vector<Object*> ObjectVector;

struct IntersectionData
{
    float tNear;
    Vector3f normal;
    Vector3f phit;
    const Object * object;
};

class Object
{   
public:
    Object()
    {
        c_diffuse = {1.0f};
        c_specular = {1.0f};
        k_ambient = 0.05f;
        k_diffuse = 1.0f;
        k_specular = 0.5f;
        shininess = 30.0f;
    }

    virtual bool intersection(const Ray& ray, IntersectionData &inter) const = 0;
    virtual bool intersection(const Ray& ray, float &tNear) const = 0;

    virtual ~Object(){}

    //Object attributes down here

    Vector3f c_diffuse;
    Vector3f c_specular;

    float k_diffuse;
    float k_specular;
    float k_ambient;

    float shininess;

    Vector3f diffColor() const
    {
        return k_diffuse * c_diffuse;
    }
    Vector3f specColor() const
    {
        return k_specular * c_specular;
    }
    Vector3f ambient() const
    {
        return k_ambient * c_diffuse;
    }
};

#endif // OBJECT_H
