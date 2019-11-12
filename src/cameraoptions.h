#ifndef CAMERAOPTIONS_H
#define CAMERAOPTIONS_H

#include "vector.h"

#define FOV 	0.698131700797732 // 40 degree
#define WIDTH 	640
#define HEIGHT 	480

class CameraOptions
{
    friend class Camera;

public:
    CameraOptions();

    Vector3  getFrom()      const { return from; }
    Vector3  getTo()        const { return to; }
    float    getFov()       const { return fov; }
    size_t   getWidth()     const { return width; }
    size_t   getHeight()    const { return height; }
    float    getRatio()     const { return aspectRatio; }

    void     setFrom(const Vector3 &value);
    void     setTo(const Vector3 &value);
    void     setFov(float value);
    void     setWidth(const size_t &value);
    void     setHeight(const size_t &value);

    friend std::ostream &operator <<(std::ostream &os, const CameraOptions &opt);

protected:
    Vector3  from;
    Vector3  to;
    float    fov;
    size_t   width;
    size_t   height;
    float    aspectRatio;
};

#endif // CAMERAOPTIONS_H
