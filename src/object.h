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

    Material()
    {
        kd = Color::WHITE;
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
public:
    Object(){ tex = std::shared_ptr<Texture>(new SolidWhite); }
    virtual ~Object() = default;

    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;
    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
    virtual const std::pair<float, float> uv(const Vector3 &, size_t) const { return std::make_pair(0.0f,0.0f); }

    const std::shared_ptr<Texture> & getTexture() const { return tex; }
    void setTexture(const std::shared_ptr<Texture> &tex) { if (tex != nullptr) this->tex = tex; }
    const Material & getMaterial() const { return material; }
    void setMaterial(const Material &material) { this->material = material; }

private:
    std::shared_ptr<Texture> tex;
    Material material;
};

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

#endif // OBJECT_H
