#include "shape.h"
#include "invisibleshape.h"

bool InstancedShape::intersection(const Ray &ray, IntersectionData &isec) const
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

bool InstancedShape::intersection(const Ray &ray, float &tnear) const
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

const Vector3 InstancedShape::normal(const Vector3 &phit, size_t idx) const
{
    return (inverseTranspose * shape->normal(inverse * phit, idx)).normalize();
}

const std::pair<float, float> InstancedShape::uv(const Vector3 &phit, size_t idx) const
{
    return shape->uv(inverse * phit, idx);
}

InstancedShape::InstancedShape(std::shared_ptr<Shape> shape)
{
    this->shape = shape ? shape : InvisibleShape::GetInstance();
}

void Instance::setShape(std::shared_ptr<Shape> shape)
{
    if(shape) this->shape = shape;
}

Instance::Instance(std::shared_ptr<Shape> shape):
    InstancedShape(shape)
{

}
