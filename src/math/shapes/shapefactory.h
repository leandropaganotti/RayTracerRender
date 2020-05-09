#pragma once

#include <memory>

#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "box.h"
#include "mesh.h"
#include "cylinder.h"

class Shapes
{
public:
    static std::shared_ptr<GSphere>      CreateSphere    (const Vector3 &center={0.0f}, const float &radius=1.0f);
    static std::shared_ptr<GPlane>       CreatePlane     (const Vector3 &O={0.0f}, const Vector3 &N={0.0f, 1.0f, 0.0f});
    static std::shared_ptr<GMesh>        CreateMesh      ();
    static std::shared_ptr<GEllipsoid>   CreateEllipsoid ();
    static std::shared_ptr<GBox>         CreateBox       ();
    static std::shared_ptr<GCylinder>    CreateCylinder  ();
};
