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
    virtual void setTransformation(const Matrix4 &transformation);

    const Matrix4& getModel()               const;
    const Matrix4& getInverse()             const;
    const Matrix4& getInverseTranspose()    const;


    /**
      TBD
    */
    /*virtual void transform(const Matrix4 &M);
    virtual void translate(const Vector3 &T);
    virtual void scale(const Vector3 &S);
    virtual void rotateX(float x);
    virtual void rotateY(float y);
    virtual void rotateZ(float z);*/

private:
    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
};

inline
const Matrix4 &Transformation::getModel() const
{
    return model;
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
