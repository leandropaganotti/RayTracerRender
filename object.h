#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <vector>
#include "vector.h"
#include "ray.h"

class Object;

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

struct IntersectionData
{
    float tnear;
    size_t idx;
    const Object * object;
};

class Object
{   
public:
    Object(const Vector3f &color)
    {
        k_diffuse = color;
        k_specular = 1.0f;
        k_ambient = k_diffuse * 0.1f;

        shininess = 150.0f;
        reflectivity = 0.3f;
        refractiveIndex = 1.0f;

        type = Type::OPAQUE;
    }   
    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;
    virtual const Vector3f normal(const Vector3f &phit, size_t idx) const = 0;

    virtual ~Object() = default;

    //Object attributes down here
    enum class Type { OPAQUE, REFRACTIVE };

    Vector3f k_diffuse;
    Vector3f k_specular;
    Vector3f k_ambient;
    float shininess;
    float reflectivity;
    float refractiveIndex;
    Type type;

};

#endif // OBJECT_H
