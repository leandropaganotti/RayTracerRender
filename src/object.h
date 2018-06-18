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
    Vector3f phit;
    Vector3f normal;
};

struct Material
{
    enum class Type { DIFFUSE, SPECULAR, MIRROR, TRANSPARENT };

    Vector3f kDiffuse;
    Vector3f kSpecular;
    float specularHighlight;
    float shininess;
    float reflectivity;
    float refractiveIndex;
    Type type;

    Material(const Vector3f &color={1.0})
    {
        kDiffuse = color;
        kSpecular = 1.0f;
        specularHighlight = 1.0f;

        shininess = 150.0f;
        reflectivity = 0.3f;
        refractiveIndex = 1.55f;

        type = Type::DIFFUSE;
    }
};

class Object
{   
public:
    Object(const Vector3f &color={1.0}): material(color) { }
    virtual ~Object() = default;

    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;
    virtual const Vector3f normal(const Vector3f &phit, size_t idx) const = 0;

    virtual const Vector3f texture(const Vector3f &, size_t) const
    {
    	return Vector3f(1.0f);
    }

    Material material;
};

#endif // OBJECT_H