#include "pathtracer.h"
#include "color.h"
#include "material.h"
#include "sphere.h"

const float bias = 0.001f;

inline float PowerHeuristic(int nf, float fPdf, int ng, float gPdf) {
    float f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}

PathTracer::PathTracer(const RenderOptions &renderOptions) : RayTracer(renderOptions) {}

Vector3 PathTracer::trace(const Ray &ray, const uint8_t depth)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;
    if(!scene->intersection(ray, isec)) return renderOptions.bgColor;
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.object->getIsecData(isec);
    isec.wo = ray.direction;

    Vector3 L(0.0);

    // light emission
    if (depth == 1)
    {
        L += isec.emittance();
    }
    if(depth > 5 && dis(gen) > 0.5)
    {
        return 0;
    }

    // direct light
    for (const auto &light : scene->lights) {
        LightData ilight;
        light->getLightData(isec.phit, ilight);
        if (ilight.pdf > 0) {
            Ray r(isec.phit + bias * isec.normal, ilight.direction, ilight.distance - 0.01);
            if (scene->intersection(r)) continue;

            Vector3 f = isec.bsdf(ilight.direction);
            if (f > 0.0)
            {
                ScatterData iscat;
                isec.bsdf(iscat);
                float weight = PowerHeuristic(1.f, ilight.pdf, 1.f, iscat.pdf);
                float cost = std::max(0.0f, ilight.direction ^ isec.normal);
                L += f* cost * ilight.Li * weight / ilight.pdf;
            }
        }
    }

    // indirect light
    ScatterData iscat;
    isec.bsdf(iscat);

    if (iscat.pdf > 0.0 ) {
        float cosTheta = isec.normal ^ iscat.scattered.direction;
        Vector3 Li = trace(iscat.scattered, depth + 1);
        L += iscat.f * Li  * std::fabs(cosTheta) / iscat.pdf;
    }

    return L;
}
