#include "instance.h"
#include "invisible.h"
#include "float.h"
#include "material.h"

Instance::Instance(std::shared_ptr<Shape> shape)
{
    this->shape = shape ? shape : InvisibleShape::GetInstance();
    updateAABB();
}

Instance::~Instance() {}

bool Instance::intersection(const Ray &ray, IntersectionData &isec) const
{
    Ray ray_local = inverse * ray;
    IntersectionData isec_local;
    if (shape->intersection(ray_local, isec_local))
    {
        Vector3 phit_local = ray_local.origin + isec_local.tnear * ray_local.direction;
        Vector3 phit = model * phit_local;
        float tnear = (phit - ray.origin).length();
        if(tnear < ray.tmax)
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

bool Instance::intersection(const Ray &ray) const
{
    Ray ray_local = inverse * ray;
    IntersectionData isec_local;
    if (shape->intersection(ray_local, isec_local))
    {
        Vector3 phit_local = ray_local.origin + isec_local.tnear * ray_local.direction;
        Vector3 phit = model * phit_local;
        float tnear = (phit - ray.origin).length();
        if(tnear < ray.tmax)
        {
            return true;
        }
    }
    return false;
}

inline
void Instance::getNormal(IntersectionData &isec) const
{
    std::swap(isec.phit, isec.phit_local);
    shape->getNormal(isec);
    isec.normal = (inverseTranspose * isec.normal).normalize();
    std::swap(isec.phit, isec.phit_local);
}

void Instance::getUV(IntersectionData &isec) const
{
    std::swap(isec.phit, isec.phit_local);
    shape->getUV(isec);
    std::swap(isec.phit, isec.phit_local);
}

void Instance::getIsecData(const Ray &, IntersectionData &isec) const
{
    std::swap(isec.phit, isec.phit_local);
    shape->getNormal(isec);
    isec.normal = (inverseTranspose * isec.normal).normalize();
    isec.material = getMaterial(isec.idx);
    isec.color = isec.material->Kd;
    if(isec.material->texture){
        shape->getUV(isec);
        isec.color = isec.color * isec.material->texture->get(isec.uv);
    }
    std::swap(isec.phit, isec.phit_local);
}

AABB Instance::getAABB() const
{
    return aabb;
}

inline
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
    aabb.reset(corners);
}
