#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "vector.h"
#include "matrix.h"
#include "image.h"

#define FOV 	0.698131700797732 // 40 degree
#define WIDTH 	640
#define HEIGHT 	480

class CameraOptions
{
    friend class Camera;
    friend class Render;

    Vector3f from;
    Vector3f to;
    float    fov;
    size_t   width;
    size_t   height;
    float    aspectRatio;

public:
    CameraOptions(const Vector3f& from={0}, const Vector3f& to={0,0,-1}, float fov=FOV, size_t width=WIDTH, size_t height=HEIGHT);

    Vector3f getFrom() const;
    void     setFrom(const Vector3f &value);
    Vector3f getTo() const;
    void     setTo(const Vector3f &value);
    float    getFov() const;
    void     setFov(float value);
    size_t   getWidth() const;
    void     setWidth(const size_t &value);
    size_t   getHeight() const;
    void     setHeight(const size_t &value);
    float    getAspectRatio() const;

    friend std::ostream &operator <<(std::ostream &os, const CameraOptions &opt);
};

class Camera
{

protected:
    CameraOptions 	options;
    Matrix4x4f 		cameraToWorld;
    Image			image;

    Vector3f getRayDirection(float i, float j) const;

public:

    Camera();

    void lookAt(const Vector3f &from, const Vector3f &to, const Vector3f& up={0.0f,1.0f,0.0f}); 

    const CameraOptions& getOptions() const;
    void setOptions(const CameraOptions& options);

    const Vector3f& getPosition() const;   
    size_t getWidth() const;
    size_t getHeight() const;

    void setResolution(size_t width, size_t height);
    void setFov(float fov);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);
};

inline
Vector3f Camera::getRayDirection(float i, float j) const
{
    float Px = (2.0f * ((j) / options.width) - 1.0f) * tan(options.fov / 2.0f ) * options.aspectRatio;
    float Py = (1.0f - 2.0f * ((i) / options.height)) * tan(options.fov / 2.0f);
    Vector3f dir = (cameraToWorld * Vector3f(Px, Py, -1.0f)) - options.from;
    return dir.normalize();
}

#endif // CAMERA_H