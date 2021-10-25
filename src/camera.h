#pragma once

#include "matrix.h"
#include "image.h"
#include "scene.h"
#include "cameraoptions.h"

class RayTracer;

class Camera
{
public:
    Camera();
    ~Camera();
    void lookAt(const Vector3 &from, const Vector3 &to, const Vector3& up=vector::UP);

    const CameraOptions& getOptions()   const { return options; }
    const Vector3& getPosition()        const { return options.from; }
    size_t getWidth()                   const { return options.width; }
    size_t getHeight()                  const { return options.height; }
    float  getFov()                     const { return options.fov; }
    float  getRatio()                   const { return options.aspectRatio; }

    void setRenderOptions(const RenderOptions& renderOptions);
    void setOptions(const CameraOptions& options);
    void setResolution(size_t width, size_t height);
    void setWidth(float width);
    void setHeight(float height);
    void setFov(float fov);

    Ray getRay(float i, float j) const;
    std::shared_ptr<Image> capture(const Scene& scene);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);

protected:
    CameraOptions               options;
    Matrix4                     cameraToWorld;
    std::shared_ptr<Image>      buffer;
    std::unique_ptr<RayTracer>  rayTracer;
};

inline Ray Camera::getRay(float i, float j) const
{
    float Px = (2.0f * ((j) / options.width) - 1.0f) * tan(options.fov / 2.0f ) * options.aspectRatio;
    float Py = (1.0f - 2.0f * ((i) / options.height)) * tan(options.fov / 2.0f);
    Vector3 dir = (cameraToWorld * Vector3(Px, Py, -1.0f)) - options.from;
    return Ray(options.from, dir.normalize());
}
