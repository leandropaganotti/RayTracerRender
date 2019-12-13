#include "shapefactory.h"

const std::shared_ptr<Shape> Shapes::Invisible  = Shapes::GetInvisible();
const std::shared_ptr<Shape> Shapes::UnitSphere = Shapes::CreateSphere();
const std::shared_ptr<Shape> Shapes::UnitBox    = Shapes::CreateAABox();

std::shared_ptr<Shape> Shapes::GetInvisible()
{
    return InvisibleShape::GetInstance();
}

std::shared_ptr<Sphere> Shapes::CreateSphere(const Vector3 &center, const float &radius)
{
    return std::shared_ptr<Sphere>(new Sphere(center, radius));
}

std::shared_ptr<Plane> Shapes::CreatePlane(const Vector3 &O, const Vector3 &N)
{
    return std::shared_ptr<Plane>(new Plane(O, N));
}

std::shared_ptr<AABox> Shapes::CreateAABox(const Vector3 &min, const Vector3 &max)
{
    return std::shared_ptr<AABox>(new AABox(min, max));
}

std::shared_ptr<Mesh> Shapes::CreateMesh()
{
    return std::shared_ptr<Mesh>(new Mesh());
}

std::shared_ptr<Instance> Shapes::CreateInstance(std::shared_ptr<Shape> shape)
{
    return std::shared_ptr<Instance>(new Instance(shape));
}

std::shared_ptr<Instance> Shapes::CreateInstance(const std::__cxx11::string &name)
{

}

std::shared_ptr<Instance> Shapes::CreateInstanceBox()
{
    return std::shared_ptr<Instance>(new Instance(Shapes::UnitBox));
}

std::shared_ptr<Instance> Shapes::CreateInstanceSphere()
{
    return std::shared_ptr<Instance>(new Instance(Shapes::UnitSphere));
}

std::shared_ptr<Instance> Shapes::CreateInstanceMesh(std::shared_ptr<Mesh> mesh)
{
    return std::shared_ptr<Instance>(new Instance(mesh));
}
