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

    Vector3 from;
    Vector3 to;
    float    fov;
    size_t   width;
    size_t   height;
    float    aspectRatio;

public:
    CameraOptions(const Vector3& from={0}, const Vector3& to={0,0,-1}, float fov=FOV, size_t width=WIDTH, size_t height=HEIGHT);

    Vector3 getFrom() const;
    void     setFrom(const Vector3 &value);
    Vector3 getTo() const;
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

class Camera
{

protected:
    CameraOptions 	options;
    Matrix4  		cameraToWorld;
    Image			image;

    Vector3 getRayDirection(float i, float j) const;

public:

    Camera();

    void lookAt(const Vector3 &from, const Vector3 &to, const Vector3& up={0.0f,1.0f,0.0f}); 

    const CameraOptions& getOptions() const;
    void setOptions(const CameraOptions& options);

    const Vector3& getPosition() const;   
    size_t getWidth() const;
    size_t getHeight() const;

    void setResolution(size_t width, size_t height);
    void setFov(float fov);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);
};

inline
Vector3 Camera::getRayDirection(float i, float j) const
{
    float Px = (2.0f * ((j) / options.width) - 1.0f) * tan(options.fov / 2.0f ) * options.aspectRatio;
    float Py = (1.0f - 2.0f * ((i) / options.height)) * tan(options.fov / 2.0f);
    Vector3 dir = (cameraToWorld * Vector3(Px, Py, -1.0f)) - options.from;
    return dir.normalize();
}

#endif // CAMERA_H
