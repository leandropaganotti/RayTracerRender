#include "phong.h"
#include "material.h"
#include "color.h"

const float bias = 0.001f;

Vector3 Phong::trace(const Ray &ray, const uint8_t depth, float E)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;

    if (!castRay(ray, isec))
        return renderOptions.bgColor;

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
            Ray r;
            r.origin = isec.phit + bias * isec.normal;
            r.direction = reflect(ray.direction, isec.normal).normalize();
            reflected = trace(r, depth + 1, E);
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
    Vector3 phitColor(0);

    for(auto& light: scene->lights)
    {

        LightData isecLight;
        light->getLightData(isec.phit, isecLight);

        float vis = light->visibility(Ray(isec.phit + bias * isec.normal, -isecLight.direction), scene->objects);

        //diffuse
        Vector3 diffuse = vis * isecLight.intensity * (isec.normal ^ -isecLight.direction);

        //specular
        Vector3 toCamera = -ray.direction;
        Vector3 reflected = reflect(isecLight.direction, isec.normal);
        Vector3 specular = vis * powf(std::max(0.0f, toCamera ^ reflected), isec.material->Ns);

        phitColor += (diffuse*isec.color + specular*isec.material->Ks);

    }
    return isec.color * isec.material->Ka + phitColor;
}
