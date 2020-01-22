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

class Object: public ShapeWraper
{
public:    
    Object(std::shared_ptr<ShapeIF> shape=nullptr, std::shared_ptr<Material> material=nullptr)
    {
        setShape(shape);
        setMaterial(material);        
    }
    Vector3 color(const IntersectionData& isec) const;

    void setShape(std::shared_ptr<ShapeIF> shape) { this->shape = shape? shape : Shapes::Invisible;}
    const ShapeIF * getShape() const { return shape.get(); }

    const Material * getMaterial() const { return material.get(); }
    void setMaterial(const std::string &name);
    void setMaterial(const std::shared_ptr<Material> material);

protected:
    std::shared_ptr<Material> material;    
};

inline
Vector3 Object::color(const IntersectionData &isec) const
{
    const Vector2 uv = shape->uv(isec.phit, isec.idx);
    return material->getTexture()->get(uv) * material->Kd;
}
inline
void Object::setMaterial(const std::string &name)
{
    if(!(material = Material::GetByName(name)))
        material = Material::DiffuseWhite;
}
inline
void Object::setMaterial(const std::shared_ptr<Material> material)
{
    this->material = material ? material : Material::DiffuseWhite;
}

#endif // OBJECT_H
