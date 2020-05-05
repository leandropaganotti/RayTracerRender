#include "shape.h"

void Shape::setTransformation(const Matrix4 &)
{
    std::cout << "setTransformation not implemented" << std::endl;
}

ShapeWithMaterial::ShapeWithMaterial(): mat(Material::DiffuseWhite){}

ShapeWithMaterial::~ShapeWithMaterial(){}

std::shared_ptr<Material> ShapeWithMaterial::getMaterial() const
{
    return mat;
}

void ShapeWithMaterial::setMaterial(const std::shared_ptr<Material> &value)
{
    mat = value ? value : Material::DiffuseWhite;
}

const Material *ShapeWithMaterial::material(const Vector3 &, size_t) const
{
    return mat.get();
}

bool Instance::intersection(const Ray &ray, IntersectionData &isec) const
{
    Ray r = inverse * ray;
    if (shape->intersection(r, isec))
    {
        Vector3 phit = r.origin + isec.tnear * r.direction;
        phit = model * phit;
        isec.tnear = (phit - ray.origin).length();
        return true;
    }
    else
        return false;
}

bool Instance::intersection(const Ray &ray, float &tnear) const
{
    Ray r = inverse * ray;
    if (shape->intersection(r, tnear))
    {
        Vector3 phit = r.origin + tnear * r.direction;
        phit = model * phit;
        tnear = (phit - ray.origin).length();
        return true;
    }
    else
        return false;
}

Vector3 Instance::normal(const Vector3 &phit, size_t idx) const
{
    return (inverseTranspose * shape->normal(inverse * phit, idx)).normalize();
}

Vector2 Instance::uv(const Vector3 &phit, size_t idx) const
{
    return shape->uv(inverse * phit, idx);
}

Instance::Instance(std::shared_ptr<Shape> shape)
{
    this->shape = shape;
}

void Instance::setTransformation(const Matrix4 &transformation)
{        
    model = transformation;
    inverse = model.getInverse();
    inverseTranspose = inverse.getTranspose();
}

const Material *Instance::material(const Vector3 &phit, size_t idx) const
{
    return shape->material(phit, idx);
}
