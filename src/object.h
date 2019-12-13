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

class Object: public Shape
{
public:    
    Object(std::shared_ptr<Shape> shape=nullptr, std::shared_ptr<Material> material=nullptr, std::shared_ptr<Texture> texture=nullptr)
    {
        setShape(shape);
        setMaterial(material);
        setTexture(texture);
    }

    const Vector3 color(const IntersectionData& isec) const;

    const Shape * getShape() const { return shape.get(); }
    void setShape(const std::string &name);
    void setShape(std::shared_ptr<Shape> shape);

    Material * getMaterial() const { return material.get(); }
    void setMaterial(const std::string &name);
    void setMaterial(std::shared_ptr<Material> material);

    Texture * getTexture() const { return texture.get(); }
    void setTexture(const std::string &name);
    void setTexture(std::shared_ptr<Texture> texture);

    // IntersectionIF interface
public:
    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    Vector3 normal(const Vector3 &phit, size_t idx) const override;
    std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override;

    // TransformationIF interface
public:
    void setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale);
    void setTransformation(const Matrix4 &transformation);

protected:
    std::shared_ptr<Shape>    shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<Texture>  texture;
};

inline
Vector3 Object::normal(const Vector3 &phit, size_t idx) const
{
    return shape->normal(phit, idx);
}
inline
std::pair<float, float> Object::uv(const Vector3 &phit, size_t idx) const
{
    return shape->uv(phit, idx);
}
inline
void Object::setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{
    shape->setTransformation(translate, rotate, scale);
}
inline
void Object::setTransformation(const Matrix4 &transformation)
{
    shape->setTransformation(transformation);
}
inline
bool Object::intersection(const Ray &ray, IntersectionData &isec) const
{
    return shape->intersection(ray, isec);
}
inline
bool Object::intersection(const Ray &ray, float &tnear) const
{
    return shape->intersection(ray, tnear);
}
inline
const Vector3 Object::color(const IntersectionData &isec) const
{
    const std::pair<float, float> _uv = shape->uv(isec.phit, isec.idx);
    return texture->get(_uv.first, _uv.second) * material->kd;
}
inline
void Object::setShape(std::shared_ptr<Shape> shape)
{
    this->shape = shape ? shape : Shapes::Invisible;
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
