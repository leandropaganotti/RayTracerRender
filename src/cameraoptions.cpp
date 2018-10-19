#include "cameraoptions.h"
#include "utils.h"

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
