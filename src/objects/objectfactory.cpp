#include "objectfactory.h"
#include "sphere.h"
#include "plane.h"
#include "box.h"
#include "cylinder.h"
#include "mesh.h"
#include "objparser.h"
#include "disk.h"

std::shared_ptr<Object> ObjectFactory::CreateSphereStatic(const Vector3 &position, const float radius, std::shared_ptr<Material> &material)
{
    return std::make_shared<SimpleObject>(std::make_shared<Sphere>(position, radius), material);
}

std::shared_ptr<Object> ObjectFactory::CreateSphere(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedSimpleObject>(primitives::UnitSphere, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreatePlaneStatic(const Vector3 &origin, const Vector3 &normal, std::shared_ptr<Material> &material)
{
    return std::make_shared<SimpleObject>(std::make_shared<Plane>(origin, normal), material);
}

std::shared_ptr<Object> ObjectFactory::CreatePlane(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedSimpleObject>(primitives::XYPlane, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateDisk(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return nullptr;
}

std::shared_ptr<Object> ObjectFactory::CreateDiskStatic(const Vector3 &origin, const Vector3 &normal, float radius, std::shared_ptr<Material> &material)
{
    return std::make_shared<SimpleObject>(std::make_shared<Disk>(origin, normal, radius), material);
}

std::shared_ptr<Object> ObjectFactory::CreateBox(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedSimpleObject>(primitives::UnitBox, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateCylinder(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedSimpleObject>(primitives::UnitCylinder, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateMesh(const std::string &src, std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    auto mesh = Resource<Mesh>::Get(src);
    if(!mesh)
    {
        mesh = OBJParser::ParseMesh(src);
        if (!mesh)
            return nullptr;

        Resource<Mesh>::Add(src, mesh);
    }

   return std::make_shared<TransformedSimpleObject>(mesh, material, transform);
}
