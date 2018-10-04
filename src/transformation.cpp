#include "transformation.h"
#include "utils.h"
#include <string.h>

void Transformation::setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{
    modelMatrix = Matrix4::T(translate) * Matrix4::Rz(rotate.z) * Matrix4::Ry(rotate.y) * Matrix4::Rx(rotate.x) * Matrix4::S(scale);
    inverse = modelMatrix.getInverse();
    inverseTranspose = inverse.getTranspose();
}
