#pragma once

#include <memory>
#include "list.h"
#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "box.h"
#include "invisibleshape.h"
#include "mesh.h"
#include "cylinder.h"


class Shapes
{
    static List<Shape> ListOfNamedShapes;
public:
    static std::shared_ptr<Shape>       Get(const std::string &name);

    static std::shared_ptr<Sphere>      CreateSphere    (const Vector3 &center={0.0f}, const float &radius=1.0f, const std::string &name="");
    static std::shared_ptr<Plane>       CreatePlane     (const Vector3 &O={0.0f}, const Vector3 &N={0.0f, 1.0f, 0.0f}, const std::string &name="");
    static std::shared_ptr<AABox>       CreateAABox     (const Vector3 &min={-0.5f, -0.5f, 0.5f}, const Vector3 &max={0.5f,0.5f,-0.5f}, const std::string &name="");
    static std::shared_ptr<Mesh>        CreateMesh      (const std::string &name="");

    static std::shared_ptr<Shape>       CreateInstance  (const std::shared_ptr<Shape> shape);
    static std::shared_ptr<Shape>       CreateInstance  (const std::string &name);

    static std::shared_ptr<Shape>       CreateEllipsoid ();
    static std::shared_ptr<Shape>       CreateBox       ();
    static std::shared_ptr<Shape>       CreateCylinder  ();

    // pre defined shapes
    const static std::shared_ptr<Shape> Invisible;
    const static std::shared_ptr<Shape> UnitSphere;
    const static std::shared_ptr<Shape> UnitBox;        
    const static std::shared_ptr<Shape> UnitCylinder;
};
