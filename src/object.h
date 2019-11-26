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


class Object
{           
public:
    Object()
    {
        material = Material::DiffuseWhite;
        texture = Texture::SolidWhite;
    }
    virtual ~Object() = default;

    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;
    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;

    virtual const std::pair<float, float> uv(const Vector3 &, size_t) const = 0;

    const Vector3 color(const IntersectionData& isec) const
    {
        const std::pair<float, float> _uv = uv(isec.phit, isec.idx);
        return texture->get(_uv.first, _uv.second) * material->kd;
        return material->kd;
    }        
    const Material * getMaterial() const { return material.get(); }
    void setMaterial(const std::string &name)
    {
        if(!(material = Material::GetByName(name)))
            material = Material::DiffuseWhite;
    }
    void setMaterial(std::shared_ptr<const Material>m)
    {
        material = m ? m : Material::DiffuseWhite;
    }

    const Texture * getTexture() const { return texture.get(); }
    void setTexture(const std::string &name)
    {
        if(!(texture = Texture::GetByName(name)))
            texture = Texture::SolidWhite;
    }
    void setTexture(std::shared_ptr<const Texture> tex)
    {
        texture = tex ? tex : Texture::SolidWhite;
    }

protected:
    std::shared_ptr<const Texture> texture;

    std::shared_ptr<const Material> material;

};

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

#endif // OBJECT_H
