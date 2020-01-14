#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <vector>
#include "shape.h"
#include "material.h"
#include "texture.h"
#include "shapefactory.h"

class Object;

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

class Object: public Instance
{
public:    
    Object(std::shared_ptr<Shape> shape=nullptr, std::shared_ptr<Material> material=nullptr, std::shared_ptr<Texture> texture=nullptr)
    {
        setShape(shape);
        setMaterial(material);
        setTexture(texture);
    }
    const Vector3 color(const IntersectionData& isec) const;

    void setShape(std::shared_ptr<Shape> shape) { this->shape = shape;}
    const Shape * getShape() const { return shape.get(); }

    Material * getMaterial() const { return material.get(); }
    void setMaterial(const std::string &name);
    void setMaterial(std::shared_ptr<Material> material);

    Texture * getTexture() const { return texture.get(); }
    void setTexture(const std::string &name);
    void setTexture(std::shared_ptr<Texture> texture);   

protected:
    std::shared_ptr<Material> material;
    std::shared_ptr<Texture>  texture;
};

inline
const Vector3 Object::color(const IntersectionData &isec) const
{
    const Vector2 _uv = shape->uv(isec.phit, isec.idx);
    return texture->get(_uv.u, _uv.v) * material->kd;
}
inline
void Object::setMaterial(const std::string &name)
{
    if(!(material = Material::GetByName(name)))
        material = Material::DiffuseWhite;
}
inline
void Object::setMaterial(std::shared_ptr<Material> material)
{
    this->material = material ? material : Material::DiffuseWhite;
}
inline
void Object::setTexture(const std::string &name)
{
    if(!(texture = Texture::GetByName(name)))
        texture = Texture::SolidWhite;
}
inline
void Object::setTexture(std::shared_ptr<Texture> texture)
{
    this->texture = texture ? texture : Texture::SolidWhite;
}

#endif // OBJECT_H
