#include "shapefactory.h"

List<ShapeIF> Shapes::ListOfNamedShapes;

const std::shared_ptr<ShapeIF> Shapes::Invisible  = std::shared_ptr<ShapeIF> (new InvisibleShape);
const std::shared_ptr<ShapeIF> Shapes::UnitSphere = std::shared_ptr<ShapeIF> (new Sphere);
const std::shared_ptr<ShapeIF> Shapes::UnitBox    = std::shared_ptr<ShapeIF> (new AABox);
const std::shared_ptr<ShapeIF> Shapes::UnitCylinder    = std::shared_ptr<ShapeIF> (new UnitYCylinder);

std::shared_ptr<ShapeIF> Shapes::Get(const std::string &name)
{
    return Shapes::ListOfNamedShapes.get(name);
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

std::shared_ptr<Mesh> Shapes::CreateMesh(const std::string &name)
{
    std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    if (name != "")
        ListOfNamedShapes.add(mesh, name);
    return mesh;
}

std::shared_ptr<ShapeIF> Shapes::CreateEllipsoid()
{
    return CreateInstance(UnitSphere);
}

std::shared_ptr<ShapeIF> Shapes::CreateBox()
{
    return CreateInstance(UnitBox);
}

std::shared_ptr<ShapeIF> Shapes::CreateCylinder()
{
    return CreateInstance(UnitCylinder);
}

std::shared_ptr<ShapeIF> Shapes::CreateInstance(const std::shared_ptr<ShapeIF> shape)
{
    return std::shared_ptr<ShapeIF>(new LocalInstance(shape));
}
