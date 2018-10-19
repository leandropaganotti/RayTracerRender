#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "vector.h"
#include "matrix.h"
#include "image.h"
#include "scene.h"
#include "cameraoptions.h"

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

    virtual void capture(const Scene &scene) = 0;

    const CameraOptions& getOptions() const;
    void setOptions(const CameraOptions& options);

    const Vector3& getPosition() const;   
    size_t getWidth() const;
    size_t getHeight() const;
    const Image& getImage()  const;

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
