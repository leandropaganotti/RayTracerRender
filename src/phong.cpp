#include "phong.h"
#include "material.h"
#include "color.h"

const float bias = 0.001f;

Phong::Phong(const RenderOptions &renderOptions) : RayTracer(renderOptions) {}

Vector3 Phong::trace(const Ray &ray, const uint8_t depth)
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
        float kr = schlick(ray.direction, isec.normal, isec.material->R0);
        float kt = 1.0f - kr;
        Vector3 diffused(0), reflected(0);
        if (kr > 0.0001f)
        {
            reflected = trace(Ray(isec.phit + bias * isec.normal, reflect(ray.direction, isec.normal).normalize()), depth + 1);
        }
        if (kt > 0.0001f)
            diffused = phongShading(ray, isec);

        return kt*diffused + kr*reflected;
    }
    else if (type == MaterialType::TRANSPARENT)
    {
        return transparentMaterial(ray, depth, isec);
    }
    return color::BLACK;
}
Vector3 Phong::phongShading(const Ray &ray, const IntersectionData &isec)
{
    //ambient
    Vector3 L(isec.albedo * isec.material->Ka); // starts with ambient component

    for(auto& light: scene->lights)
    {
        LightData isecLight;
        light->getLightData(isec.phit, isecLight);

        float vis = light->visibility(Ray(isec.phit + bias * isec.normal, -isecLight.direction), scene);

        //diffuse
        Vector3 diffuse = isecLight.Li * std::max(0.0f, (isec.normal ^ -isecLight.direction));

        //specular
        Vector3 reflected = reflect(isecLight.direction, isec.normal);
        Vector3 specular = powf(std::max(0.0f, -ray.direction ^ reflected), isec.material->Ns);

        L += vis * (diffuse*isec.albedo + specular*isec.material->Ks);
    }
    return L;
}
