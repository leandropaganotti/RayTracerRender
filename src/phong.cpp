#include "phong.h"
#include "material.h"
#include "color.h"

const float bias = 0.001f;

Vector3 Phong::Li(const Ray &ray, const uint8_t depth, float E)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;
    if(!scene->intersection(ray, isec)) return renderOptions.bgColor;
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.object->getIsecData(isec);

    const MaterialType type = isec.material->type;

    if (type == MaterialType::DIFFUSE)
    {
        return phongShading(ray, isec);
    }
    else if (type == MaterialType::SPECULAR)
    {
        float kr = schlick(-ray.direction, isec.normal, isec.material->R0);
        float kt = 1.0f - kr;
        Vector3 diffused(0), reflected(0);
        if (kr > 0.0001f)
        {
            reflected = Li(Ray(isec.phit + bias * isec.normal, reflect(ray.direction, isec.normal).normalize()), depth + 1, E);
        }
        if (kt > 0.0001f)
            diffused = phongShading(ray, isec);

        return kt*diffused + kr*reflected;
    }
    else if (type == MaterialType::TRANSPARENT)
    {
        return transparentMaterial(ray, depth, isec, E);
    }
    return color::BLACK;
}
Vector3 Phong::phongShading(const Ray &ray, const IntersectionData &isec)
{
    Vector3 diff, spec;
    SampleLi sample;
    for(const auto& light: scene->lights)
    {
        light->getSample(isec.phit, sample);

        if(light->visibility(Ray(isec.phit + bias * isec.normal, sample.direction, sample.distance), scene) > 0.0f)
        {
            //diffuse
            diff += sample.Li * std::max(0.0f, (isec.normal ^ sample.direction));

            //specular
            Vector3 r = reflect(-sample.direction, isec.normal);
            spec += sample.Li * powf(std::max(0.0f, -ray.direction ^ r), isec.Ns());
        }
    }
    return isec.Ka() + (diff * isec.Kd() + spec * isec.Ks());
}
