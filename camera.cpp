#include "camera.h"
#include "float.h"
#include "utils.h"
#include <thread>

void Camera::render_1x1x1(const Scene *scene, size_t start, size_t end)
{
    if (scene == nullptr) return;
    Ray ray(pos, 0.0f);
    for (size_t i = start; i < end; ++i)
    {
        for (size_t j = 0; j < frameBuffer.width(); ++j)
        {
            ray.direction = rayDirection(i, j);
            frameBuffer.at(i, j) = rayCaster.cast(ray, *scene);
        }
    }
}

void Camera::render(const Scene &scene, uint8_t nrays, uint8_t nthreads)
{
    if (nthreads ==0 || nthreads > 4)
        throw std::invalid_argument("# of threads must be 1, 2, 3 or 4");

    if (nrays < 1)
        throw std::invalid_argument("# of rays must be > 0");

    if (nrays > 1)
    {
        frameBuffer.resize( frameBuffer.width() * nrays, frameBuffer.height() * nrays);
    }

    std::vector<std::thread> threads;

    size_t nrows = frameBuffer.height() / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( &Camera::render_1x1x1, this, &scene, i*nrows, (i+1) * nrows ) );
    }
    threads.push_back( std::thread( &Camera::render_1x1x1, this, &scene, i*nrows, frameBuffer.height() ) );

    for (auto& thread : threads) thread.join();

    if( nrays > 1)
    {
        Image frame;
        frame.resize( frameBuffer.width() / nrays, frameBuffer.height() / nrays);

        for (size_t i = 0; i < frame.height(); ++i)
        {
            for (size_t j = 0; j < frame.width(); ++j)
            {
                frame.at(i, j) = 0;
                for (size_t k = i*nrays; k < i*nrays + nrays; ++k)
                {
                    for (size_t l = j*nrays; l < j*nrays + nrays; ++l)
                    {
                        frame.at(i, j) += frameBuffer.at(k, l);
                    }
                }
                frame.at(i, j) /= nrays*nrays;
            }
        }
        frameBuffer.move(frame);
    }
}

void Camera::lookAt(const Vector3f &from, const Vector3f &to, const Vector3f &up)
{

    Vector3f zaxis = (from - to).normalize();
    Vector3f xaxis = (up % zaxis).normalize();
    Vector3f yaxis = zaxis % xaxis;

    //std::cout << zaxis << xaxis << yaxis << std::endl;

    cameraToWorld[0][0] = xaxis.x;
    cameraToWorld[1][0] = xaxis.y;
    cameraToWorld[2][0] = xaxis.z;

    cameraToWorld[0][1] = yaxis.x;
    cameraToWorld[1][1] = yaxis.y;
    cameraToWorld[2][1] = yaxis.z;

    cameraToWorld[0][2] = zaxis.x;
    cameraToWorld[1][2] = zaxis.y;
    cameraToWorld[2][2] = zaxis.z;

    cameraToWorld[0][3] = from.x;
    cameraToWorld[1][3] = from.y;
    cameraToWorld[2][3] = from.z;

    //std::cout << cameraToWorld << std::endl;

    pos = cameraToWorld * Vector3f(0.0f);
}

std::ostream &operator <<(std::ostream &os, const Camera &cam)
{
    os << "Camera parameters:" << cam.getPosition() << "\n";

    return os;
}


