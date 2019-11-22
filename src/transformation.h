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

    const Matrix4& getModel()               const { return model;  }
    const Matrix4& getInverse()             const { return inverse;}
    const Matrix4& getInverseTranspose()    const { return inverseTranspose; }


    /**************************************************
      TODO
    **************************************************/
/*
    virtual void transform(const Matrix4 &M) = 0;
    virtual void translate(const Vector3 &T) = 0;
    virtual void scale(const Vector3 &S) = 0;
    virtual void rotateX(float x) = 0;
    virtual void rotateY(float y) = 0;
    virtual void rotateZ(float z) = 0;
    virtual void rotateZ(float x, float y, float z) = 0;
*/

private:
    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
};

#endif // TRANSFORMATION_H
