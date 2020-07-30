#include "pathtracer.h"
#include "color.h"
#include "material.h"
#include "sphere.h"

const float bias = 0.001f;

Vector3 PathTracer::Li(const Ray &ray, const uint8_t depth, const float E)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;
    if(!scene->intersection(ray, isec)) return renderOptions.bgColor;
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.object->getIsecData(isec);

    MaterialType type = isec.material->type;

    if (type == MaterialType::DIFFUSE)
    {
        /*
         * A Lambertian surface by definition reflects radiance equally into all directions. Its BRDF is simply

            $BRDF = ~= rho / M_PI,

            where $ \rho$ is called the reflectivity of the surface (from 0 to 1).

            Lo = E + (BRDF * Li * cos) / pdf; where pdf = 1/Area = 1/2M_PI (hemisphere)

            Lo = E + albedo/M_PI * trace() * cos * 1/pdf
            Lo = E + albedo/M_PI * trace() * cos * 2M_PI
            Lo = E + albedo * trace() * cos * 2
        */
        Ray r(isec.phit + bias * isec.normal, randomUnitVectorInHemisphereOf(isec.normal));
        float cosTheta = isec.normal ^ r.direction;
        return isec.material->E + isec.albedo * Li(r, depth+1, E) * cosTheta * 2.0f;
    }
    else if (type == MaterialType::SPECULAR)
    {
        float kr = schlick(-ray.direction, isec.normal, isec.material->R0);
        float kt = 1.0f - kr;
        Vector3 diffused(0), reflected(0);
        if (kr > 0.0001f)
        {
            Ray r(isec.phit + bias * isec.normal, reflect(ray.direction, isec.normal).normalize());
            reflected = Li(r, depth+1, 0);
        }
        if (kt > 0.0001f)
        {
            Ray r(isec.phit + bias * isec.normal, randomUnitVectorInHemisphereOf(isec.normal));
            float cosTheta = isec.normal ^ r.direction;
            diffused = isec.material->E + isec.albedo * Li(r, depth+1, E) * cosTheta * 2.0f;
        }

        return kt*diffused + kr*reflected;
    }
    else if (type == MaterialType::TRANSPARENT)
    {
        return transparentMaterial(ray, depth, isec, E);
    }
    return color::BLACK;
}

Vector3 PathTracerWithDirectSampling::Li(const Ray &ray, const uint8_t depth, const float E)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;
    if(!scene->intersection(ray, isec)) return renderOptions.bgColor;
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.object->getIsecData(isec);

    const MaterialType type = isec.material->type;

    float kt=1.0f, kr=0.0f; // default diffuse material values
    if(type == MaterialType::TRANSPARENT)
    {
        return transparentMaterial(ray, depth, isec, E);
    }
    else if (type == MaterialType::SPECULAR)
    {
        kr = schlick(-ray.direction, isec.normal, isec.material->R0);
        kt = 1.0f - kr;
    }else if(type == MaterialType::LIGHT){

        float n=5, m=1;
        float nk = isec.normal ^ -ray.direction;
        float ek = (n+1)/(2*M_PI)*powf(nk, m);
        Vector3 Lo = isec.material->E * ek / nk;
        return Lo;
    }

    Vector3 diffused, reflected;
    Vector3 f = isec.albedo * M_1_PI;
    if (kt > 0.0001f)
    {
        Vector3 direct(0);
        //direct light
        for(auto &light : scene->lights)
        {
            SampleLi sample;
            Vector3 p = isec.phit + bias * isec.normal;
            light->getSample(p, sample);

            float vis = light->visibility(Ray(p, sample.direction, sample.distance), scene);

            //diffuse
            direct  += vis * f * sample.Li * std::abs((isec.normal ^ sample.direction)) * sample._1_pdf;
        }
        direct.x = clamp(direct.x);
        direct.y = clamp(direct.y);
        direct.z = clamp(direct.z);
        //indirect light
        const float _1_pdf = (2.0f*M_PI);//1/(2*M_PI)
        Ray r(isec.phit + bias * isec.normal, randomUnitVectorInHemisphereOf(isec.normal));
        float cosine = isec.normal ^ r.direction;
        Vector3 indirect = f * Li(r, depth+1, 0.0f) * cosine * _1_pdf;

        diffused = direct + indirect;
    }

    if(kr > 0.0001f)
    {
        Ray r(isec.phit + bias * isec.normal, reflect(ray.direction, isec.normal).normalize());
        reflected = Li(r, depth+1, E);
    }

    return kt*diffused + kr*reflected;
}
