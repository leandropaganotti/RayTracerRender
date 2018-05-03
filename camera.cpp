#include "camera.h"
#include "float.h"
#include "utils.h"
#include <thread>

void thread_worker(Camera *camera, const Scene *scene, size_t start, size_t end, size_t nrays)
{
    camera->render1(*scene, start, end);
}

Camera::Camera(): origin(0), fov(40), width(640), height(480)
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

    if (nrays > 1)
    {
        resolution(width * nrays, height * nrays);
    }

    std::vector<std::thread> threads;

    size_t nrows = height / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( thread_worker, this, &scene, i*nrows, (i+1) * nrows, nrays ) );
    }

    threads.push_back( std::thread( thread_worker, this, &scene, i*nrows, height, nrays ) );

    for (auto& t : threads) t.join();

    if( nrays > 1)
    {
        Image frame;
        frame.resize(width / nrays, height / nrays);

        for (size_t i = 0; i < frame.height; ++i)
        {
            for (size_t j = 0; j < frame.width; ++j)
            {
                frame.at(i, j) = 0;
                for (size_t k = i*nrays; k < i*nrays + nrays; ++k)
                {
                    for (size_t l = j*nrays; l < j*nrays + nrays; ++l)
                    {
                        frame.at(i, j) += m_frame.at(k, l);
                    }
                }
                frame.at(i, j) /= nrays*nrays;
            }
        }
        m_frame.move(frame);
        width = m_frame.width;
        height = m_frame.height;
        aspectRatio = (float)width / (float)height;
    }
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
