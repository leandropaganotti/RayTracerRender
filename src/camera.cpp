#include "camera.h"

Camera::Camera()
{
    lookAt(options.from, options.to);
    image.resize(options.width, options.height);
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

const Vector3& Camera::getPosition() const
{
    return options.from;
}

const CameraOptions& Camera::getOptions() const
{
    return options;    
}

size_t Camera::getWidth() const
{
    return options.width;
}

size_t Camera::getHeight() const
{
    return options.height;
}

void Camera::setResolution(size_t width, size_t height)
{
    options.width = width;
    options.height = height;
    options.aspectRatio = float(width) / height;
    image.resize(options.width, options.height);
}

void Camera::setFov(float fov)
{
    options.fov = fov;
}

void Camera::setOptions(const CameraOptions& options)
{
    this->options = options;
    lookAt(options.from, options.to);
    image.resize(options.width, options.height);
}

std::ostream &operator <<(std::ostream &os, const Camera &cam)
{
    return os << "Camera: " << cam.options;
}

CameraOptions::CameraOptions(const Vector3 &from, const Vector3 &to, float fov, size_t width, size_t height):
    from(from), to(to), fov(fov), width(width), height(height), aspectRatio(float(width)/height)
{}

Vector3 CameraOptions::getTo() const
{
    return to;
}

void CameraOptions::setTo(const Vector3 &value)
{
    to = value;
}

float CameraOptions::getFov() const
{
    return fov;
}

void CameraOptions::setFov(float value)
{
    fov = value;
}

size_t CameraOptions::getWidth() const
{
    return width;
}

void CameraOptions::setWidth(const size_t &value)
{
    width = value;
    aspectRatio = float(width) / height;
}

size_t CameraOptions::getHeight() const
{
    return height;
}

void CameraOptions::setHeight(const size_t &value)
{
    height = value;
    aspectRatio = float(width) / height;
}

float CameraOptions::getAspectRatio() const
{
    return aspectRatio;
}

Vector3 CameraOptions::getFrom() const
{
    return from;
}

void CameraOptions::setFrom(const Vector3 &value)
{
    from = value;
}

std::ostream &operator <<(std::ostream &os, const CameraOptions &opt)
{
    return os << "CameraOptions: " << opt.from << opt.to << " " << rad2deg(opt.fov) << " " << opt.width << " " << opt.height << " " << opt.aspectRatio;
}
