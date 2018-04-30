#include "camera.h"
#include "float.h"
#include "utils.h"

Camera::Camera(): origin(0), fov(40), width(250), height(250)
{
    m_frame.resize(width, height);
    aspectRatio = (float)width / (float)height;
    fov = deg2rad(fov);
}

Camera::~Camera() { }

void Camera::resolution(size_t w, size_t h)
{
    width = w;
    height = h;
    m_frame.resize(width, height);
    aspectRatio = (float)width / (float)height;
}

Image &Camera::render4(const Scene &scene)
{
    Ray ray;
    width *= 2;
    height*= 2;
    size_t m=0, n=0;
    ray.origin = origin = cameraToWorld * origin;
    for (size_t i = 0; i < height; i=i+2)
    {
        n=0;
        for (size_t j = 0; j < width; j=j+2)
        {
            ray.direction = rayDirection(i, j);
            m_frame.at(m, n)  = rayCaster.cast(ray, scene);

            ray.direction = rayDirection(i, j+1);
            m_frame.at(m, n) += rayCaster.cast(ray, scene);

            ray.direction = rayDirection(i+1, j);
            m_frame.at(m, n) += rayCaster.cast(ray, scene);

            ray.direction = rayDirection(i+1, j+1);
            m_frame.at(m, n) += rayCaster.cast(ray, scene);

            m_frame.at(m, n) = m_frame.at(m, n) / 4.0f;
            ++n;
        }
        ++m;
    }
    return m_frame;
}

Image &Camera::render1(const Scene &scene)
{
    Ray ray;
    ray.origin = origin = cameraToWorld * origin;
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            ray.direction = rayDirection(i, j);
            m_frame.at(i, j) = rayCaster.cast(ray, scene);
        }
    }
    return m_frame;
}

std::ostream &operator <<(std::ostream &os, const Camera &cam)
{
    os << "Camera parameters:" << "\n";
    os << "|Origin: " << cam.origin << "\n";
    os << "|Image size: " << cam.width << "x" << cam.height  << " - " << "aspectRatio: " << cam.aspectRatio << "\n";
    os << "|fov: " << cam.fov << "\n";
    return os;
}

inline
Vector3f Camera::rayDirection(size_t i, size_t j)
{
    float Px = (2.0f * ((j + 0.5f) / width) - 1.0f) * tan(fov / 2.0f ) * aspectRatio;
    float Py = (1.0f - 2.0f * ((i + 0.5f) / height)) * tan(fov / 2.0f);
    Vector3f dir = cameraToWorld * Vector3f(Px, Py, -1.0f);
    return (dir - origin).normalize();
}
