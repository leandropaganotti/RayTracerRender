#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iostream>
#include <math.h>

template<typename T>
struct Vector3;

typedef Vector3<float> Vector3f;

template<typename T>
struct Vector3
{
    union
    {
        T vec[3];
        struct {T x, y, z;};
        struct {T r, g, b;};
    };

	Vector3() = default;
	Vector3(T x): x(x), y(x), z(x){}
	Vector3(T x, T y, T z): x(x), y(y), z(z){}
    
    T& operator[](size_t i) { return vec[i]; }
    const T& operator[](size_t i) const { return vec[i]; }
    
    T dot(const Vector3& v) const
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
    bool operator==(const Vector3f& rhs) const
    {
        return x==rhs.x && y==rhs.y && z==rhs.z;
    }
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
    Vector3 operator+(const Vector3& rhs) const
    {
        return {x+rhs.x, y+rhs.y, z+rhs.z};
    }
    void operator+=(const Vector3& rhs)
    {
        x+=rhs.x; y+=rhs.y; z+=rhs.z;
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
    Vector3 operator%(const Vector3& rhs) const //cross
    {
        return cross(rhs);
    }

    friend std::ostream& operator<< (std::ostream& os, const Vector3& v)
    {
        return os << std::fixed << "[ " << v.x << " " << v.y << " " << v.z <<  " ]";
    }
};

#endif // VECTOR_H
