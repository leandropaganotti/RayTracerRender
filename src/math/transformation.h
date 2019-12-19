#pragma once

#include "vector.h"
#include "matrix.h"

class TransformationIF
{    
public:
    TransformationIF() = default;
    virtual ~TransformationIF() = default;

    virtual void setTransformation(const Vector3 &, const Vector3 &, const Vector3 &)
    {
        std::cout << "Not Implemented yet";
    }
};


class Transformation: public Matrix4 {

public:

    static Transformation T(const Vector3 &v);
    static Transformation S(const Vector3 &v);
    static Transformation R(const Vector3 &v);
    static Transformation Rx(float a);
    static Transformation Ry(float a);
    static Transformation Rz(float a);

    Transformation & applyT(const Vector3 &v);
    Transformation & applyS(const Vector3 &v);
    Transformation & applyR(const Vector3 &v);
};
