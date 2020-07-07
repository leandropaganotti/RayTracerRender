#include "raytracer.h"
#include <typeinfo>
#include <thread>
#include <random>
#include <atomic>
#include <iomanip>
#include <sstream>
#include <iostream>
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
        return std::make_shared<PathTracerWithDirectSampling>();
    case Illumination::GlobalIlumination:
        return std::make_shared<PathTracer>();
    case Illumination::Phong:
        return std::make_shared<Phong>();
    default:
        return std::make_shared<Minimum>();
    }
}

RayTracer::RayTracer()
{

}

RayTracer::~RayTracer(){}

void RayTracer::render(const Scene& scene)
{
    this->scene = &scene;

    int sqrt_samples = sqrt(renderOptions.spp);
    int spp = sqrt_samples * sqrt_samples;
    std::vector<Vector2> samples = rng(spp);
    filter(samples);

    std::cout << " -> SPP: " << spp << std::endl << std::endl;

    std::cout << "\r -> 0.00% completed" << std::flush;

    size_t count = 0, total = camera.getWidth()*camera.getHeight();
    buffer.resize(camera.getWidth(), camera.getHeight());
    for (size_t i = 0; i < camera.getHeight(); ++i)
    {

        #ifndef DEBUG
        #pragma omp parallel for schedule(dynamic, 1) shared(count)
        #endif
        for (size_t j = 0; j < camera.getWidth(); ++j)
        {
            buffer.at(i, j) = 0;
            for (auto &sample: samples)
            {
                Ray ray(camera.getRay(i+sample.y, j+sample.x));
                buffer.at(i, j) += trace(ray, 1, 1.0f);
            }
            buffer.at(i, j) /= spp;
            ++count;

        }
        std::cout << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(total) * 100.0f << "% completed" << std::flush;
    }
    std::cout << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(total) * 100.0f << "% completed" << std::flush;
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

        Ray T(phit - bias * normal, (n * ray.direction + (n * cosi - cost) * normal).normalize());
        transmited = trace(T, depth + 1, E) * kt;
    }

    // reflection
    if(kr)
    {
        Ray R(phit + bias * normal, reflect(ray.direction, normal).normalize());
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
    if(scene->intersection(ray, isec))
    {
        //return isec.material->Kd;
        isec.phit = ray.origin + isec.tnear * ray.direction;
        isec.object->getIsecData(isec);
        return isec.albedo;
    }
    return renderOptions.bgColor;
}

