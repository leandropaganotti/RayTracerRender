#include "shape.h"
#include "shapefactory.h"
#include <float.h>

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
inline
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
            return true;
        }
    }
    return false;
}

bool Instance::intersection(const Ray &ray, float tmax) const
{
    IntersectionData isec;
    return intersection(ray, tmax, isec);
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
    this->shape = shape ? shape : Shapes::Invisible;
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

void Instance::fetch(const Ray &, IntersectionData &isec) const
{
    Vector3 phit = inverse * isec.phit;

    isec.material = mat.get();
    if(mat->texture)
    {
        isec.uv = shape->uv(phit, isec.idx);
        isec.color = mat->texture->get(isec.uv) * mat->Kd;
    }
    else
        isec.color = mat->Kd;

   isec.normal = (inverseTranspose * shape->normal(phit, isec.idx)).normalize();

}
