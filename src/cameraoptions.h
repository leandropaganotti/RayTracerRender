#ifndef CAMERAOPTIONS_H
#define CAMERAOPTIONS_H

#include "vector.h"

#define FOV 	0.698131700797732 // 40 degree
#define WIDTH 	640
#define HEIGHT 	480

class CameraOptions
{
    friend class Camera;
    friend class Render;

    Vector3  from;
    Vector3  to;
    float    fov;
    size_t   width;
    size_t   height;
    float    aspectRatio;

public:
    CameraOptions(const Vector3& from={0}, const Vector3& to={0,0,-1}, float fov=FOV, size_t width=WIDTH, size_t height=HEIGHT);

    Vector3  getFrom() const;
    void     setFrom(const Vector3 &value);
    Vector3  getTo() const;
    void     setTo(const Vector3 &value);
    float    getFov() const;
    void     setFov(float value);
    size_t   getWidth() const;
    void     setWidth(const size_t &value);
    size_t   getHeight() const;
    void     setHeight(const size_t &value);
    float    getAspectRatio() const;

    friend std::ostream &operator <<(std::ostream &os, const CameraOptions &opt);
};

#endif // CAMERAOPTIONS_H
