#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iostream>
#include <math.h>

template<typename T> struct Vector3;

typedef Vector3<float> Vector3f;

template<typename T>
struct Vector3
{
    Vector3(){}
    Vector3(T x): x(x), y(x), z(x){}
    Vector3(T x, T y, T z): x(x), y(y), z(z){}

    T           dot(const Vector3& v)    const { return this->x*v.x + this->y*v.y + this->z*v.z; }
    Vector3     cross(const Vector3 & v) const { return {this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.x}; }
    float       length()                 const { return sqrtf(this->x*this->x + this->y*this->y + this->z*this->z); }
    float       dist(const Vector3& v)   const { return sqrtf(v.x*this->x + v.y*this->y +v.z*this->z); }
    Vector3&	normalize(){ float m = length(); this->x/=m; this->y/=m; this->z/=m; return *this;}

    Vector3 operator-() const
    {
        return {-x, -y, -z};
    }
    Vector3 operator+(const Vector3& rhs) const
    {
        return {x+rhs.x, y+rhs.y, z+rhs.z};
    }
    void operator+=(const Vector3& rhs)
    {
        x+=rhs.x; y+=rhs.y; z+=rhs.z;
    }
    Vector3 operator-(const Vector3& rhs) const
    {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }
    void operator-=(const Vector3& rhs)
    {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
    }
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
    float operator^(const Vector3& rhs) const //dot
    {
        return dot(rhs);
    }
    Vector3 operator%(const Vector3& rhs) const
    {
        return cross(rhs);
    }

    union
    {
        T vec[3];
        struct {T x, y, z;};
        struct {T r, g, b;};
    };
    T& operator[](size_t i) { return vec[i]; }
    const T& operator[](size_t i) const { return vec[i]; }
    friend std::ostream& operator<< (std::ostream& os, const Vector3& v)
    {
        return os << std::fixed << "[ " << v.x << " " << v.y << " " << v.z <<  " ]";
    }
};

#endif // VECTOR_H
