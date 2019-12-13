#pragma once

#include <memory>
#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "aabox.h"
#include "invisibleshape.h"
#include "mesh.h"

class Shapes
{
public:
    static std::shared_ptr<Shape>       GetInvisible();

    static std::shared_ptr<Sphere>      CreateSphere            (const Vector3 &center={0.0f}, const float &radius=1.0f);
    static std::shared_ptr<Plane>       CreatePlane             (const Vector3 &O={0.0f}, const Vector3 &N={0.0f, 0.1f, 0.0f});
    static std::shared_ptr<AABox>       CreateAABox             (const Vector3 &min={-0.5f, -0.5f, 0.5f}, const Vector3 &max={0.5f,0.5f,-0.5f});
    static std::shared_ptr<Mesh>        CreateMesh              ();

    static std::shared_ptr<Instance>    CreateInstance          (std::shared_ptr<Shape> shape);
    static std::shared_ptr<Instance>    CreateInstance          (const std::string &name);
    static std::shared_ptr<Instance>    CreateInstanceBox       ();
    static std::shared_ptr<Instance>    CreateInstanceSphere    ();
    static std::shared_ptr<Instance>    CreateInstanceMesh      (std::shared_ptr<Mesh> mesh);

    // pre defined shapes
    const static std::shared_ptr<Shape> Invisible;
    const static std::shared_ptr<Shape> UnitSphere;
    const static std::shared_ptr<Shape> UnitBox;
};
