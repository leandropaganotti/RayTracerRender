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
public:
    Camera();
    void lookAt(const Vector3 &from, const Vector3 &to, const Vector3& up=Vector::UP);
    virtual void capture(const Scene &scene) = 0;

    const CameraOptions& getOptions()   const { return options; }
    const Vector3& getPosition()        const { return options.from; }
    size_t getWidth()                   const { return options.width; }
    size_t getHeight()                  const { return options.height; }
    float  getFov()                     const { return options.fov; }
    float  getRatio()                   const { return options.aspectRatio; }
    const Image& getBuffer()            const { return buffer; }

    void setOptions(const CameraOptions& options);
    void setResolution(size_t width, size_t height);
    void setWidth(float width);
    void setHeight(float height);
    void setFov(float fov);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);

protected:
    CameraOptions 	options;
    Matrix4  		cameraToWorld;
    Image			buffer;
};

#endif // CAMERA_H
