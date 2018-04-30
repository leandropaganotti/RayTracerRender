#ifndef CAMERA_H
#define CAMERA_H

#include "raycaster.h"
#include "vector.h"
#include "scene.h"
#include "image.h"
#include "matrix.h"


class Camera
{
    float fov;

    Image m_frame;

    size_t width;
    size_t height;
    float aspectRatio;

    RayCaster rayCaster;

    Vector3f rayDirection(size_t i, size_t j);

public:
    Vector3f origin;

    Camera();

    ~Camera();

    void resolution(size_t w, size_t h);

    void render(const Scene &scene, uint8_t nrays=1, uint8_t nthreads=1);
    void render1(const Scene &scene, size_t start, size_t end);
    void render4(const Scene &scene, size_t start, size_t end);

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);

    Matrix4x4f cameraToWorld;

    const Image& frame() const
    {
        return m_frame;
    }


};

#endif // CAMERA_H
