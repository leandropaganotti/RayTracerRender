#include "shapefactory.h"

List<Shape> Shapes::ListOfNamedShapes;

const std::shared_ptr<Shape> Shapes::Invisible  = Shapes::GetInvisible();
const std::shared_ptr<Shape> Shapes::UnitSphere = Shapes::CreateSphere();
const std::shared_ptr<Shape> Shapes::UnitBox    = Shapes::CreateAABox();

std::shared_ptr<Shape> Shapes::GetInvisible()
{
    return InvisibleShape::GetInstance();
}

std::shared_ptr<Sphere> Shapes::CreateSphere(const Vector3 &center, const float &radius, const std::string &name)
{
    std::shared_ptr<Sphere> sphere = std::shared_ptr<Sphere>(new Sphere(center, radius));
    if (name != "")
        ListOfNamedShapes.add(sphere, name);
    return sphere;
}

std::shared_ptr<Plane> Shapes::CreatePlane(const Vector3 &O, const Vector3 &N,const std::string &name)
{
    std::shared_ptr<Plane> plane = std::shared_ptr<Plane>(new Plane(O, N));
    if (name != "")
        ListOfNamedShapes.add(plane, name);
    return plane;
}

std::shared_ptr<AABox> Shapes::CreateAABox(const Vector3 &min, const Vector3 &max, const std::string &name)
{
    std::shared_ptr<AABox> aabox = std::shared_ptr<AABox>(new AABox(min, max));
    if (name != "")
        ListOfNamedShapes.add(aabox, name);
    return aabox;
}

std::shared_ptr<Mesh> Shapes::CreateMesh(const std::__cxx11::string &name)
{
    std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    if (name != "")
        ListOfNamedShapes.add(mesh, name);
    return mesh;
}

std::shared_ptr<Instance> Shapes::CreateInstance(std::shared_ptr<Shape> shape)
{
    return std::shared_ptr<Instance>(new Instance(shape));
}

std::shared_ptr<Instance> Shapes::CreateInstance(const std::string &name)
{
    return std::shared_ptr<Instance>(new Instance(ListOfNamedShapes.get(name)));
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
