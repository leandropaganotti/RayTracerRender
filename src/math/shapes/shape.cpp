#include "shape.h"
#include "shapefactory.h"

void Instance::setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{
    shape->setTransformation(translate, rotate, scale);
}

bool Instance::intersection(const Ray &ray, IntersectionData &isec) const
{    
    return shape->intersection(ray, isec);
}

bool Instance::intersection(const Ray &ray, float &tnear) const
{
    return shape->intersection(ray, tnear);
}

Vector3 Instance::normal(const Vector3 &phit, size_t idx) const
{
    return shape->normal(phit, idx);
}

std::pair<float, float> Instance::uv(const Vector3 &phit, size_t idx) const
{
    return shape->uv(phit, idx);
}

Instance::Instance(std::shared_ptr<Shape> shape)
{
    this->shape = shape ? shape : Shapes::Invisible;
}

bool LocalInstance::intersection(const Ray &ray, IntersectionData &isec) const
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

bool LocalInstance::intersection(const Ray &ray, float &tnear) const
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

Vector3 LocalInstance::normal(const Vector3 &phit, size_t idx) const
{
    return (inverseTranspose * shape->normal(inverse * phit, idx)).normalize();
}

std::pair<float, float> LocalInstance::uv(const Vector3 &phit, size_t idx) const
{
    return shape->uv(inverse * phit, idx);
}

LocalInstance::LocalInstance(std::shared_ptr<Shape> shape): Instance(shape)
{

}

void LocalInstance::setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{        
    model = Transformation::TSR(translate, rotate, scale);
    inverse = model.getInverse();
    inverseTranspose = inverse.getTranspose();
}
