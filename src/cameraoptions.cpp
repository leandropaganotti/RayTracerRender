#include "cameraoptions.h"
#include "utils.h"

#define FOV 	0.698131700797732 // 40 degree
#define WIDTH 	640
#define HEIGHT 	480

CameraOptions::CameraOptions():
        from(0), to(vector::FRONT), fov(FOV), width(WIDTH), height(HEIGHT), aspectRatio(float(WIDTH)/HEIGHT)
{}

void CameraOptions::setTo(const Vector3 &value)
{
    to = value;
}

void CameraOptions::setFov(float value)
{
    fov = value;
}

void CameraOptions::setWidth(const size_t &value)
{
    width = value;
    aspectRatio = float(width) / height;
}

void CameraOptions::setHeight(const size_t &value)
{
    height = value;
    aspectRatio = float(width) / height;
}

void CameraOptions::setFrom(const Vector3 &value)
{
    from = value;
}

std::ostream &operator <<(std::ostream &os, const CameraOptions &opt)
{
    return os << "CameraOptions: From=" << opt.from << ", To=" << opt.to << ", Fov=" << rad2deg(opt.fov) << ", W=" << opt.width << ", H=" << opt.height << ", Ratio=" << opt.aspectRatio;
}
