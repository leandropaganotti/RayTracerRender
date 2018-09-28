#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "vector.h"
#include "matrix.h"

class Transformation
{    
public:
    Transformation() = default;
    virtual ~Transformation() = default;

    virtual void setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale);
    virtual void setTransformation(const Matrix4& transformation);

    const Matrix4& getTransformation() const;

    const Matrix4& getInverse() const;

    const Matrix4& getInverseTranspose() const;

    static Matrix4 T(const Vector3& v);

    static Matrix4 S(const Vector3& v);

    static Matrix4 Rx(float angle);

    static Matrix4 Ry(float angle);

    static Matrix4 Rz(float angle);

protected:
    Matrix4 transformation;      // object-to-world
    Matrix4 inverse;             // world-to-object
    Matrix4 inverseTranspose;    // matrix for normals transformation
};

inline
const Matrix4 &Transformation::getTransformation() const
{
    return transformation;
}
inline
const Matrix4 &Transformation::getInverse() const
{
    return inverse;
}
inline
const Matrix4 &Transformation::getInverseTranspose() const
{
    return inverseTranspose;
}

#endif // TRANSFORMATION_H
