#include "raytracer.h"
#include <typeinfo>
#include <thread>
#include <random>
#include <atomic>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "consts.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"
#include "color.h"
#include "pathtracer.h"
#include "phong.h"

const float bias = 0.001f;

std::shared_ptr<RayTracer> RayTracer::Create(Illumination illum)
{
    switch (illum) {
    case Illumination::GlobalIluminationWithDirectSampling:
        return std::shared_ptr<RayTracer> (new PathTracerWithDirectSampling);
    case Illumination::GlobalIlumination:
        return std::shared_ptr<RayTracer> (new PathTracer);
    case Illumination::Phong:
        return std::shared_ptr<RayTracer> (new Phong);
    default:
        return std::shared_ptr<RayTracer> (new Minimum);
    }
}

RayTracer::RayTracer()
{

}

RayTracer::~RayTracer(){}

void RayTracer::render(const Scene& scene)
{
    this->scene = &scene;
    const float grid = renderOptions.gridSizeXY;
    const float gridSize = 1.0f/grid;
    const size_t nrays_persubpixel = ceilf(renderOptions.spp/(grid*grid));
    const size_t nrays_perpixel = nrays_persubpixel * grid * grid;

    int count = 0;

    buffer.resize(camera.getWidth(), camera.getHeight());

    std::cout << std::endl;
    std::cout << "\r -> 0.00% completed" << std::flush;


    for (size_t i = 0; i < camera.getHeight(); ++i)
    {
        std::ostringstream ss;
        #ifndef DEBUG
        #pragma omp parallel for schedule(dynamic, 1) shared(count)
        #endif
        for (size_t j = 0; j < camera.getWidth(); ++j)
        {
            buffer.at(i, j) = 0;
            for (size_t ii=0; ii < grid; ++ii)
            {
                unsigned short Xi[3]={0,0,(short unsigned int)(ii)};
                for (size_t jj=0; jj < grid; ++jj)
                {
                    for (size_t n = 0; n < nrays_persubpixel; ++n)
                    {
                        const float x = i + gridSize * (ii + erand48(Xi));
                        const float y = j + gridSize * (jj + erand48(Xi));
                        Ray ray(camera.getPosition(), rayDirection(x, y));
                        buffer.at(i, j) += trace(ray, 1, 1.0f);
                    }
                }
            }
            buffer.at(i, j) /= nrays_perpixel;
        }
        ++count;
        ss << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(camera.getHeight()) * 100.0f << "% completed";
        std::cout << ss.str() << std::flush;
    }

}

inline
Vector3 RayTracer::rayDirection(float i, float j) const
{
    float Px = (2.0f * ((j) / camera.getWidth()) - 1.0f) * tan(camera.getFov() / 2.0f ) * camera.getRatio();
    float Py = (1.0f - 2.0f * ((i) / camera.getHeight())) * tan(camera.getFov() / 2.0f);
    Vector3 dir = (camera.getCameraToWorld() * Vector3(Px, Py, -1.0f)) - camera.getPosition();
    return dir.normalize();
}


bool RayTracer::castRay(const Ray &ray, IntersectionData &isec)
{
    float tmax = FLT_MAX;
    isec.shape = nullptr;
    for(auto &object : scene->objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            tmax = isec.tnear;
        }
    }

    if(!isec.shape) return false;

    isec.shape->getIsecData(ray, isec);

    return true;
}


float RayTracer::castShadowRay(const Ray &ray, float tmax)
{
    float vis = 1.0f;
    IntersectionData isec;
    for(auto &object : scene->objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

inline
Vector3 RayTracer::specularReflection(const Ray &ray, const uint8_t depth, const IntersectionData &isec)
{
    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = reflect(ray.direction, isec.normal).normalize();
    return trace(R, depth + 1, 1.0f);
}

Vector3 RayTracer::transparentMaterial(const Ray &ray, const uint8_t depth, const IntersectionData &isec, float E)
{
    const Vector3 &phit = isec.phit;
    Vector3 normal = isec.normal;
    const Material &material = *isec.material;

    Vector3 transmited(0.0f), reflected(0.0f);

    // transmited
    float n, n1, n2, kr, kt;
    float cosi = normal.dot(ray.direction);
    if ( cosi < 0.0f) // outside surface
    {
        cosi = -cosi;
        n1 = renderOptions.ambientIndex;
        n2 = material.Ni;
    }
    else              // inside surface
    {
        normal = -normal;
        n1 = material.Ni;
        n2 = renderOptions.ambientIndex;
    }

    n = n1 / n2;
    float sint2 = n * n * (1.0f - cosi * cosi);

    if (sint2 > 1.0f) { kr = 1.0f; kt = 0.0f; } // TIR

    //refraction
    else
    {
        // fresnel
        float cost  = sqrt(1.0f - sint2);
        float Rs = (n1 * cosi - n2 * cost) / (n1 * cosi + n2 * cost);
        float Rp = (n2 * cosi - n1 * cost) / (n2 * cosi + n1 * cost);

        kr = (Rs * Rs + Rp * Rp) / 2.0f;
        kt = 1.0f - kr;

        Ray T;
        T.origin = phit - bias * normal;
        T.direction = n * ray.direction + (n * cosi - cost) * normal;
        T.direction.normalize();
        transmited = trace(T, depth + 1, E) * kt;
    }

    // reflection
    if(kr)
    {
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        reflected = trace(R, depth + 1, E) * kr;
    }
    return transmited + reflected;
}

void RayTracer::setCameraOptions(const CameraOptions &value)
{
    camera.setOptions(value);
}

void RayTracer::setRenderOptions(const RenderOptions &value)
{
    renderOptions = value;
}

Vector3 Minimum::trace(const Ray &ray, const uint8_t, const float)
{
    IntersectionData isec;
    if (!castRay(ray, isec))
        return renderOptions.bgColor;
    return isec.material->Kd;
}

