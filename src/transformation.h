#ifndef MODEL_H
#define MODEL_H

#include "vector.h"
#include "matrix.h"

// object-to-world
class Transformation: public Matrix4x4f
{
public:
    Transformation() = default;

    void build(const Vector3f &translate, const Vector3f &rotate, const Vector3f &scale);

    const Matrix4x4f& getInverse() const
    {
        return inverse;
    }

    const Matrix4x4f& getInverseTranspose() const
    {
        return inverseTranspose;
    }

private:
    Matrix4x4f inverse;             // world-to-object
    Matrix4x4f inverseTranspose;    // matrix for normals transformation
};

#endif // MODEL_H
