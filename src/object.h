#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <vector>
#include "vector.h"
#include "ray.h"
#include "texture.h"
#include "matrix.h"
#include "transformation.h"

class Object;

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
    enum class Type { DIFFUSE, SPECULAR, TRANSPARENT };
    Type type;
    Vector3f kd;
    Vector3f ks;
    Vector3f Le;    
    float highlight;
    float shininess;
    float reflectivity;
    float refractiveIndex;

    std::unique_ptr<Texture> tex;

    Material(const Vector3f &color={1.0})
    {
        kd = color;
        ks = 1.0f;
        Le = 0.0f;
        highlight = 0.0f;
        shininess = 30.0f;
        reflectivity = 0.0f;
        refractiveIndex = 1.55f;
        type = Type::DIFFUSE;
    }
};

class Object: public Transformation
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

typedef std::vector<std::unique_ptr<Object>> ObjectVector;


#endif // OBJECT_H
