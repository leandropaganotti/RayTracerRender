#include "camera.h"
#include "float.h"
#include "utils.h"
#include <thread>

void thread_worker(Camera *camera, const Scene *scene, size_t start, size_t end, size_t nrays)
{
    if (nrays == 4)
        camera->render4(*scene, start, end);
    else
        camera->render1(*scene, start, end);
}

Camera::Camera(): origin(0), fov(40), width(150), height(150)
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

void Camera::render4(const Scene &scene, size_t start, size_t end)
{
//    Ray ray;
//    width *= 2;
//    height*= 2;
//    size_t m=0, n=0;
//    ray.origin = origin;
//    for (size_t i = start; i < end; i=i+2)
//    {
//        n=0;
//        for (size_t j = 0; j < width; j=j+2)
//        {
//            ray.direction = cameraToWorld * rayDirection(i, j);
//            m_frame.at(m, n)  = rayCaster.cast(ray, scene);

//            ray.direction = cameraToWorld * rayDirection(i, j+1);
//            m_frame.at(m, n) += rayCaster.cast(ray, scene);

//            ray.direction = cameraToWorld * rayDirection(i+1, j);
//            m_frame.at(m, n) += rayCaster.cast(ray, scene);

//            ray.direction = cameraToWorld * rayDirection(i+1, j+1);
//            m_frame.at(m, n) += rayCaster.cast(ray, scene);

//            m_frame.at(m, n) = m_frame.at(m, n) / 4.0f;
//            ++n;
//        }
//        ++m;
//    }
}

void Camera::render1(const Scene &scene, size_t start, size_t end)
{
    Ray ray;
    ray.origin = origin;
    for (size_t i = start; i < end; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            ray.direction = cameraToWorld * rayDirection(i, j);

            m_frame.at(i, j) = rayCaster.cast(ray, scene);
        }
    }
}

void Camera::render(const Scene &scene, uint8_t nrays, uint8_t nthreads)
{
    if (nthreads ==0 || nthreads > 4)
        throw std::invalid_argument("# of threads must be 1, 2, 3 or 4");

    std::vector<std::thread> threads;

    size_t nrows = height / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( thread_worker, this, &scene, i*nrows, (i+1) * nrows, nrays ) );
    }

    threads.push_back( std::thread( thread_worker, this, &scene, i*nrows, height, nrays ) );

    for (auto& th : threads) th.join();
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
    return Vector3f(Px, Py, -1.0f).normalize();
}
