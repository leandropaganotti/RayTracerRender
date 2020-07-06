#include "objectfactory.h"
#include "sphere.h"
#include "plane.h"
#include "box.h"
#include "cylinder.h"
#include "mesh.h"
#include "objparser.h"

std::shared_ptr<Object> ObjectFactory::CreateSphereStatic(const Vector3 &position, const float radius, std::shared_ptr<Material> &material)
{
    return std::make_shared<SimpleObject>(std::make_shared<Sphere>(position, radius), material);
}

std::shared_ptr<Object> ObjectFactory::CreateSphere(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedObject>(shape::unitSphere, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreatePlaneStatic(const Vector3 &origin, const Vector3 &normal, std::shared_ptr<Material> &material)
{
    return std::make_shared<SimpleObject>(std::make_shared<Plane>(origin, normal), material);
}

std::shared_ptr<Object> ObjectFactory::CreatePlane(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedObject>(shape::xyPlane, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateBox(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedObject>(shape::unitBox, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateCylinder(std::shared_ptr<Material> &material, const Matrix4 &transform)
{
    return std::make_shared<TransformedObject>(shape::unitCylinder, material, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateMesh(std::shared_ptr<Mesh> &mesh, const Matrix4 &transform)
{
    return std::make_shared<TransformedObject>(mesh, transform);
}

std::shared_ptr<Object> ObjectFactory::CreateObjModel(const std::string &src, const Matrix4 &transform)
{
    auto mesh = Resource<Mesh>::Get(src);

    if(!mesh)
    {
        mesh = OBJParser::ParseMesh(src);
        Resource<Mesh>::Add(src, mesh);
    }
    return std::make_shared<TransformedObject>(mesh, transform);
}
