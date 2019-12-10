#include "transformation.h"
#include "utils.h"
#include <string.h>

void TransformationIF::setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{
    model = Matrix4::T(translate) * Matrix4::Rz(rotate.z) * Matrix4::Ry(rotate.y) * Matrix4::Rx(rotate.x) * Matrix4::S(scale);
    inverse = model.getInverse();
    inverseTranspose = inverse.getTranspose();
}

void TransformationIF::setTransformation(const Matrix4 &transformation)
{
    model = transformation;
    inverse = model.getInverse();
    inverseTranspose = inverse.getTranspose();
}
