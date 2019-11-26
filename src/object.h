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
#include "intersection.h"
#include "material.h"

class Object;

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

class Object: public IntersectionIF
{
public:
    Object(): material(Material::DiffuseWhite), texture(Texture::SolidWhite){}
    virtual ~Object() = default;

    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
    virtual const std::pair<float, float> uv(const Vector3 &, size_t) const = 0;

    const Vector3 color(const IntersectionData& isec) const;

    const Material * getMaterial() const { return material.get(); }
    void setMaterial(const std::string &name);
    void setMaterial(std::shared_ptr<const Material>m);

    const Texture * getTexture() const { return texture.get(); }
    void setTexture(const std::string &name);
    void setTexture(std::shared_ptr<const Texture> tex);
private:
    std::shared_ptr<const Material> material;
    std::shared_ptr<const Texture> texture;

};

#endif // OBJECT_H

inline
const Vector3 Object::color(const IntersectionData &isec) const
{
    const std::pair<float, float> _uv = uv(isec.phit, isec.idx);
    return texture->get(_uv.first, _uv.second) * material->kd;    
}

void Object::setMaterial(const std::string &name)
{
    if(!(material = Material::GetByName(name)))
        material = Material::DiffuseWhite;
}

void Object::setMaterial(std::shared_ptr<const Material> m)
{
    material = m ? m : Material::DiffuseWhite;
}

void Object::setTexture(const std::string &name)
{
    if(!(texture = Texture::GetByName(name)))
        texture = Texture::SolidWhite;
}

void Object::setTexture(std::shared_ptr<const Texture> tex)
{
    texture = tex ? tex : Texture::SolidWhite;
}
