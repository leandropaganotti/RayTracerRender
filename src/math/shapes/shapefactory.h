#pragma once

#include <memory>
#include "list.h"

#include "sphere.h"
#include "plane.h"
#include "aabox.h"
#include "mesh.h"
#include "cylinder.h"
#include "shape.h"
#include "invisibleshape.h"

class Shapes
{
    static List<ShapeIF> ListOfNamedShapes;
public:
    static std::shared_ptr<ShapeIF>       Get(const std::string &name);

    static std::shared_ptr<Sphere>      CreateSphere    (const Vector3 &center={0.0f}, const float &radius=1.0f, const std::string &name="");
    static std::shared_ptr<Plane>       CreatePlane     (const Vector3 &O={0.0f}, const Vector3 &N={0.0f, 1.0f, 0.0f}, const std::string &name="");
    static std::shared_ptr<AABox>       CreateAABox     (const Vector3 &min={-0.5f, -0.5f, 0.5f}, const Vector3 &max={0.5f,0.5f,-0.5f}, const std::string &name="");
    static std::shared_ptr<Mesh>        CreateMesh      (const std::string &name="");

    static std::shared_ptr<ShapeIF>       CreateInstance  (const std::shared_ptr<ShapeIF> shape);

    static std::shared_ptr<ShapeIF>       CreateEllipsoid ();
    static std::shared_ptr<ShapeIF>       CreateBox       ();
    static std::shared_ptr<ShapeIF>       CreateCylinder  ();

    // pre defined shapes
    const static std::shared_ptr<ShapeIF> Invisible;
    const static std::shared_ptr<ShapeIF> UnitSphere;
    const static std::shared_ptr<ShapeIF> UnitBox;
    const static std::shared_ptr<ShapeIF> UnitCylinder;
};
