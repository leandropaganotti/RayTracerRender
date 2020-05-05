#include "shapefactory.h"

//const std::shared_ptr<ShapeIF> Shapes::Invisible  = std::shared_ptr<ShapeIF> (new InvisibleShape);
const std::shared_ptr<Shape> Shapes::UnitSphere = std::shared_ptr<Shape> (new Sphere);
const std::shared_ptr<Shape> Shapes::UnitBox    = std::shared_ptr<Shape> (new AABox);
const std::shared_ptr<Shape> Shapes::UnitCylinder    = std::shared_ptr<Shape> (new UnitYCylinder);


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

std::shared_ptr<Instance> Shapes::CreateEllipsoid()
{
    return CreateInstance(UnitSphere);
}

std::shared_ptr<Instance> Shapes::CreateBox()
{
    return CreateInstance(UnitBox);
}

std::shared_ptr<Instance> Shapes::CreateCylinder()
{
    return CreateInstance(UnitCylinder);
}

std::shared_ptr<Instance> Shapes::CreateInstance(const std::shared_ptr<Shape> shape)
{
    return std::shared_ptr<Instance>(new Instance(shape));
}
