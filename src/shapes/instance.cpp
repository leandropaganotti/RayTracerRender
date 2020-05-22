#include "instance.h"
#include "invisible.h"
#include "float.h"

Instance::Instance(std::shared_ptr<Shape> shape)
{
    this->shape = shape ? shape : InvisibleShape::GetInstance();
    updateAABB();
}

Instance::~Instance() {}

bool Instance::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    Ray ray_local = inverse * ray;
    IntersectionData isec_local;
    if (shape->intersection(ray_local, FLT_MAX, isec_local))
    {
        Vector3 phit_local = ray_local.origin + isec_local.tnear * ray_local.direction;
        Vector3 phit = model * phit_local;
        float tnear = (phit - ray.origin).length();
        if(tnear < tmax)
        {
            isec.idx = isec_local.idx;
            isec.tnear = tnear;
            isec.phit = phit;
            isec.phit_local = phit_local;
            isec.shape = this;
            return true;
        }
    }
    return false;
}

bool Instance::intersection(const Ray &ray, float tmax) const
{
    Ray ray_local = inverse * ray;
    IntersectionData isec_local;
    if (shape->intersection(ray_local, FLT_MAX, isec_local))
    {
        Vector3 phit_local = ray_local.origin + isec_local.tnear * ray_local.direction;
        Vector3 phit = model * phit_local;
        float tnear = (phit - ray.origin).length();
        if(tnear < tmax)
        {
            return true;
        }
    }
    return false;
}

Vector3 Instance::getNormal(const Vector3 &phit, size_t idx) const
{
    return (inverseTranspose * shape->getNormal(inverse * phit, idx)).normalize();
}

Vector2 Instance::getUV(const Vector3 &phit, size_t idx) const
{
    return shape->getUV(inverse * phit, idx);
}

void Instance::getIsecData(const Ray &, IntersectionData &isec) const
{
    isec.normal = (inverseTranspose * shape->getNormal(isec.phit_local, isec.idx)).normalize();
}

AABB Instance::getAABB() const
{
    return aabb;
}

const Material *Instance::getMaterial(size_t idx) const
{
    return shape->getMaterial(idx);
}

void Instance::setTransformation(const Matrix4 &transformation)
{
    model = transformation;
    inverse = model.getInverse();
    inverseTranspose = inverse.getTranspose();
    updateAABB();
}

inline
void Instance::updateAABB()
{
    std::vector<Vector3> corners(8);
    Vector3 min = shape->getAABB().getMin(), max = shape->getAABB().getMax();
    corners[0] = model * min;
    corners[1] = model * Vector3(min.x, min.y, max.z);
    corners[2] = model * Vector3(min.x, max.y, min.z);
    corners[3] = model * Vector3(max.x, min.y, min.z);
    corners[4] = model * Vector3(min.x, max.y, max.z);
    corners[5] = model * Vector3(max.x, min.y, max.z);
    corners[6] = model * Vector3(max.x, max.y, min.z);
    corners[7] = model * max;
    aabb.extend(corners);
}
