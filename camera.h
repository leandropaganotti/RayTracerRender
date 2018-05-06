#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "scene.h"
#include "image.h"
#include "matrix.h"

#define FOV 40

class Camera
{
    Vector3f pos;
    Matrix4x4f cameraToWorld;
    float fov;
    size_t width;
    size_t height;
    float aspectRatio;
public:

    Camera(): pos(0), fov(deg2rad(FOV)){}

    Vector3f rayDirection(size_t i, size_t j);

    void lookAt(const Vector3f &from, const Vector3f &to, const Vector3f& up={0.0f,1.0f,0.0f});

    const Vector3f& position() const;

    void resolution(size_t width, size_t height);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);
};

inline
Vector3f Camera::rayDirection(size_t i, size_t j)
{
    float Px = (2.0f * ((j + 0.5f) / width) - 1.0f) * tan(fov / 2.0f ) * aspectRatio;
    float Py = (1.0f - 2.0f * ((i + 0.5f) / height)) * tan(fov / 2.0f);
    Vector3f dir = (cameraToWorld * Vector3f(Px, Py, -1.0f)) - pos;
    return dir.normalize();
}

#endif // CAMERA_H
