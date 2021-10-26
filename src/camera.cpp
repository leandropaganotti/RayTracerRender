#include "camera.h"
#include "raytracer.h"

Camera::Camera()
{
    lookAt(options.from, options.to);
}

void Camera::lookAt(const Vector3 &from, const Vector3 &to, const Vector3 &up)
{
    Vector3 zaxis = (from - to).normalize();
    Vector3 xaxis = (up % zaxis).normalize();
    Vector3 yaxis = zaxis % xaxis;

    cameraToWorld[0][0] = xaxis.x;
    cameraToWorld[1][0] = xaxis.y;
    cameraToWorld[2][0] = xaxis.z;

    cameraToWorld[0][1] = yaxis.x;
    cameraToWorld[1][1] = yaxis.y;
    cameraToWorld[2][1] = yaxis.z;

    cameraToWorld[0][2] = zaxis.x;
    cameraToWorld[1][2] = zaxis.y;
    cameraToWorld[2][2] = zaxis.z;

    cameraToWorld[0][3] = from.x;
    cameraToWorld[1][3] = from.y;
    cameraToWorld[2][3] = from.z;

    options.from = from;
    options.to	 = to;
}

void Camera::setResolution(size_t width, size_t height)
{
    options.width = width;
    options.height = height;
    options.aspectRatio = float(width) / height;
}

void Camera::setWidth(float width)
{
    options.width = width;
    options.aspectRatio = float(width) / options.height;
}

void Camera::setHeight(float height)
{
    options.height = height;
    options.aspectRatio = float(options.width) / height;
}

void Camera::setFov(float fov)
{
    options.fov = fov;
}

void Camera::setOptions(const CameraOptions& options)
{
    this->options = options;
    lookAt(options.from, options.to);
}

std::ostream &operator <<(std::ostream &os, const Camera &cam)
{
    return os << "Camera: " << cam.options;
}
