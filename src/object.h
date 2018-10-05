#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <vector>
#include "vector.h"
#include "ray.h"
#include "texture.h"
#include "matrix.h"
#include "transformation.h"
#include <utility>
#include "consts.h"

class Object;

struct IntersectionData
{
    float tnear;
    size_t idx;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
};

struct Material
{
    enum class Type { DIFFUSE, SPECULAR, TRANSPARENT };
    Type type;
    Vector3 kd;
    Vector3 ks;
    Vector3 Le;    
    float highlight;
    float shininess;
    float reflectivity;
    float refractiveIndex;

    Material(const Vector3 &color={1.0})
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

class Object
{       
    std::shared_ptr<Texture> tex;
    virtual const std::pair<float, float> texUV(const Vector3 &, size_t) const;

public:
    Object(const Vector3 &color={1.0}): material(color) { }
    virtual ~Object() = default;

    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;

    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
    virtual const Vector3& texture(const Vector3 &phit, size_t idx) const;

    Material material;    
    void setTexture(std::shared_ptr<Texture> tex);
};

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

inline
const std::pair<float, float> Object::texUV(const Vector3 &, size_t) const
{
    return std::make_pair(0,0);
}

inline
const Vector3 &Object::texture(const Vector3 &phit, size_t idx) const
{
    if (!tex) return Color::WHITE;

    float u, v;
    std::tie(u, v) = texUV(phit, idx);
    return tex->get(u, v) ;
}

inline
void Object::setTexture(std::shared_ptr<Texture> tex)
{
    this->tex = tex;
}

#endif // OBJECT_H
