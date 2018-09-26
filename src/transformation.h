#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "vector.h"
#include "matrix.h"

class Transformation
{    
public:
    Transformation() = default;
    virtual ~Transformation() = default;

    virtual void setTransformation(const Vector3f &translate, const Vector3f &rotate, const Vector3f &scale);
    virtual void setTransformation(const Matrix4x4f& transformation);

    const Matrix4x4f& getTransformation() const;

    const Matrix4x4f& getInverse() const;

    const Matrix4x4f& getInverseTranspose() const;

    static Matrix4x4f T(const Vector3f& v);

    static Matrix4x4f S(const Vector3f& v);

    static Matrix4x4f Rx(float angle);

    static Matrix4x4f Ry(float angle);

    static Matrix4x4f Rz(float angle);

protected:
    Matrix4x4f transformation;      // object-to-world
    Matrix4x4f inverse;             // world-to-object
    Matrix4x4f inverseTranspose;    // matrix for normals transformation
};

inline
const Matrix4x4f &Transformation::getTransformation() const
{
    return transformation;
}
inline
const Matrix4x4f &Transformation::getInverse() const
{
    return inverse;
}
inline
const Matrix4x4f &Transformation::getInverseTranspose() const
{
    return inverseTranspose;
}

#endif // TRANSFORMATION_H
