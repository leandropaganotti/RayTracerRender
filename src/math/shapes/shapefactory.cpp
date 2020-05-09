#include "shapefactory.h"

std::shared_ptr<GSphere> Shapes::CreateSphere(const Vector3 &center, const float &radius)
{
    return std::shared_ptr<GSphere>(new GSphere(center, radius));
}

std::shared_ptr<GPlane> Shapes::CreatePlane(const Vector3 &O, const Vector3 &N)
{
    return std::shared_ptr<GPlane>(new GPlane(O, N));
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
    return std::shared_ptr<Instance>(new Instance(UnitBox));
}

std::shared_ptr<Instance> Shapes::CreateCylinder()
{
    return CreateInstance(UnitCylinder);
}

std::shared_ptr<Instance> Shapes::CreateInstance(const std::shared_ptr<Shape> shape)
{
    return std::shared_ptr<Instance>(new Instance(shape));
}
