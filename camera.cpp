#include "camera.h"

void Camera::lookAt(const Vector3f &from, const Vector3f &to, const Vector3f &up)
{
    Vector3f zaxis = (from - to).normalize();
    Vector3f xaxis = (up % zaxis).normalize();
    Vector3f yaxis = zaxis % xaxis;

    //std::cout << zaxis << xaxis << yaxis << std::endl;

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

    //std::cout << cameraToWorld << std::endl;

    pos = cameraToWorld * Vector3f(0.0f);
}

const Vector3f &Camera::position() const
{
    return pos;
}

void Camera::resolution(size_t width, size_t height)
{
    this->width = width;
    this->height = height;
    aspectRatio = float(width) / height;
}

std::ostream &operator <<(std::ostream &os, const Camera &cam)
{
    return os << "Camera: " << cam.position();
}


