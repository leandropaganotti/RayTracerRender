#include "model.h"
#include "utils.h"
#include <string.h>

void ModelMatrix::build(const Vector3f &translate, const Vector3f &rotate, const Vector3f &scale)
{
    Matrix4x4f src = T(translate) * Rz(rotate.z) * Ry(rotate.y) * Rx(rotate.x) * S(scale);
    memcpy(mat, src.mat, sizeof(float) * 16);
    inverse = S(1.0f/scale) * Rx(-rotate.x) * Ry(-rotate.y) * Rz(-rotate.z) * T(-translate);
    inverseTranspose = inverse.transpose();
}
