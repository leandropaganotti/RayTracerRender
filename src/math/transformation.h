#pragma once

#include "vector.h"
#include "matrix.h"

class TransformationIF
{    
public:
    TransformationIF() = default;
    virtual ~TransformationIF() = default;

    virtual void setTransformation(const Matrix4 &)
    {
        std::cout << "Not Implemented yet";
    }
};

class Transformation {

public:
    Transformation() = delete;

    static Matrix4 T(const Vector3& v);
    static Matrix4 S(const Vector3& v);
    static Matrix4 Rx(float angle);
    static Matrix4 Ry(float angle);
    static Matrix4 Rz(float angle);

    static Matrix4 TSR(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale);
};
