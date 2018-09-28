#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iostream>
#include <math.h>



struct Vector3
{
    union
    {
        float vec[3];
        struct {float x, y, z;};
        struct {float r, g, b;};
    };

    Vector3() = default;
    Vector3(float x): x(x), y(x), z(x){}
    Vector3(float x, float y, float z): x(x), y(y), z(z){}
    
    float& operator[](size_t i) { return vec[i]; }
    const float& operator[](size_t i) const { return vec[i]; }
            
    /** Basic operations */
    // op -
    Vector3 operator-() const
    {
        return {-x, -y, -z};
    }
    Vector3 operator-(const Vector3& rhs) const
    {
	    return {x - rhs.x, y - rhs.y, z - rhs.z};
    }
    void operator-=(const Vector3& rhs)
    {
	    x -= rhs.x; y -= rhs.y; z -= rhs.z;
    }

    // op +
    Vector3 operator+(const Vector3& rhs) const
    {
        return {x+rhs.x, y+rhs.y, z+rhs.z};
    }
    void operator+=(const Vector3& rhs)
    {
        x+=rhs.x; y+=rhs.y; z+=rhs.z;
    }

    // op *
    Vector3 operator*(float scalar) const
    {
        return { scalar * x, scalar * y, scalar * z};
    }
    Vector3 operator*=(float scalar)
    {
        return { x *= scalar, y *= scalar, z *= scalar};
    }
    Vector3 operator*(const Vector3& rhs) const
    {
        return {x * rhs.x, y * rhs.y, z * rhs.z};
    }
    friend Vector3 operator*(float scalar, const Vector3& rhs)
    {
        return rhs * scalar;
    }

    // op /
    friend Vector3 operator/(float scalar, const Vector3& rhs)
    {
        return {scalar/rhs.x, scalar/rhs.y, scalar/rhs.z};
    }
    Vector3 operator/(float scalar) const
    {
        return {x/scalar, y/scalar, z/scalar};
    }
    void operator/=(float scalar)
    {
        x/=scalar; y/=scalar; z/=scalar;
    }
    Vector3 operator/(const Vector3& v) const
    {
        return {x/v.x, y/v.y, z/v.z};
    }
    void operator/=(const Vector3& v)
    {
        x/=v.x, y/=v.y, z/=v.z;
    }    

    // logical operations
    bool operator==(const Vector3& rhs) const
    {
        return x==rhs.x && y==rhs.y && z==rhs.z;
    }

    // distance
    float distance(const Vector3& rhs)
    {
        return (*this - rhs).length();
    }

    /** Math cector functions */
    float dot(const Vector3& v) const
    {
        return x*v.x + y*v.y + z*v.z;
    }
    Vector3 cross(const Vector3 & v) const
    {
        return {y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x};
    }
    float length() const
    {
        return sqrtf(x*x + y*y + z*z);
    }
    const Vector3& normalize()
    {
        float m = length(); x/=m; y/=m; z/=m; return *this;
    }
    float operator^(const Vector3& rhs) const //dot
    {
        return dot(rhs);
    }
    Vector3 operator%(const Vector3& rhs) const //cross
    {
        return cross(rhs);
    }

    /** Print function */
    friend std::ostream& operator<< (std::ostream& os, const Vector3& v)
    {
        return os << std::fixed << "[ " << v.x << " " << v.y << " " << v.z <<  " ]";
    }
};

#endif // VECTOR_H
