#pragma once

#include "vector.h"
#include "matrix.h"

class TransformationIF
{    
public:
    TransformationIF() = default;
    virtual ~TransformationIF() = default;

    virtual void setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale);
    virtual void setTransformation(const Matrix4 &transformation);

    const Matrix4& getModel()               const;
    const Matrix4& getInverse()             const;
    const Matrix4& getInverseTranspose()    const;


    /**************************************************
      TODO
    **************************************************/
    /*virtual void transform(const Matrix4 &M);
    virtual void translate(const Vector3 &T);
    virtual void scale(const Vector3 &S);
    virtual void rotateX(float x);
    virtual void rotateY(float y);
    virtual void rotateZ(float z);*/

protected:
    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
};

inline
const Matrix4 &TransformationIF::getModel() const
{
    return model;
}
inline
const Matrix4 &TransformationIF::getInverse() const
{
    return inverse;
}
inline
const Matrix4 &TransformationIF::getInverseTranspose() const
{
    return inverseTranspose;
}
