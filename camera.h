#ifndef CAMERA_H
#define CAMERA_H

#include "raycaster.h"
#include "vector.h"
#include "scene.h"
#include "image.h"
#include "matrix.h"


class Camera
{
    Vector3f pos;

    float fov;

    Image frameBuffer;

    RayCaster rayCaster;

    Matrix4x4f cameraToWorld;

    Vector3f rayDirection(size_t i, size_t j)
    {
        float Px = (2.0f * ((j + 0.5f) / frameBuffer.width()) - 1.0f) * tan(fov / 2.0f ) * frameBuffer.aspectRatio();
        float Py = (1.0f - 2.0f * ((i + 0.5f) / frameBuffer.height())) * tan(fov / 2.0f);
        Vector3f dir = (cameraToWorld * Vector3f(Px, Py, -1.0f)) - pos;
        return dir.normalize();
    }

    void render_1x1x1(const Scene *scene, size_t start, size_t end); // 1 ray per 1 pixel and 1 thread

public:

    Camera(): pos(0), fov(40)
    {
        frameBuffer.resize(640, 480);
        fov = deg2rad(fov);
    }

    ~Camera() { }

    void render(const Scene &scene, uint8_t nrays=1, uint8_t nthreads=1);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);

    const Image& getFrameBuffer() const
    {
        return frameBuffer;
    }

    const Vector3f& getPosition() const
    {
        return pos;
    }

    void lookAt(const Vector3f &from, const Vector3f &to, const Vector3f& up={0.0f,1.0f,0.0f});


};

#endif // CAMERA_H
