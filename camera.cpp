#include "camera.h"

void Camera::lookAt(const Vector3f &from, const Vector3f &to, const Vector3f &up)
{
    Vector3f zaxis = (from - to).normalize();
    Vector3f xaxis = (up % zaxis).normalize();
    Vector3f yaxis = zaxis % xaxis;

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

const Vector3f& Camera::getPosition() const
{
    return options.from;
}

const CameraOptions& Camera::getOptions() const
{
	return options;
}

void Camera::setResolution(size_t width, size_t height)
{
	options.width = width;
    options.height = height;
    options.aspectRatio = float(width) / height;
}

void Camera::setFov(float fov)
{
	options.fov = fov;
}

void Camera::setOptions(const CameraOptions& options)
{
	this->options = options;
}

std::ostream &operator <<(std::ostream &os, const Camera &cam)
{
    return os << "Camera: " << cam.options;
}

std::ostream &operator <<(std::ostream &os, const CameraOptions &opt)
{
    return os << "Camera: " << opt.from << opt.to << " " << rad2deg(opt.fov) << " " << opt.width << " " << opt.height;
}

