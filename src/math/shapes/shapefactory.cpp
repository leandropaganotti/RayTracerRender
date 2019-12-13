#include "shapefactory.h"
#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "invisibleshape.h"
#include "box.h"

const std::shared_ptr<Shape> Shapes::Invisible  = Shapes::GetInvisible();
const std::shared_ptr<Shape> Shapes::UnitSphere = Shapes::CreateSphere();
const std::shared_ptr<Shape> Shapes::UnitBox    = Shapes::CreateAABox();

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

std::shared_ptr<Box> Shapes::CreateBox()
{
    return std::shared_ptr<Box>(new Box());
}

std::shared_ptr<Instance> Shapes::CreateInstance(std::shared_ptr<Shape> shape)
{
    return std::shared_ptr<Instance>(new Instance(shape));
}

std::shared_ptr<InvisibleShape> Shapes::GetInvisible()
{
    return InvisibleShape::GetInstance();
}
