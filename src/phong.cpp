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
    //ambient
    Vector3 phitColor = isec.color * isec.material->Ka;

    for(auto& light: scene->lights)
    {
        Vector3 toLight = light->direction(isec.phit);

        float cosTheta = isec.normal ^ toLight; //NdotL
        if( cosTheta > 0.0f )
        {
            float vis = castShadowRay(Ray(isec.phit + bias * isec.normal, toLight), light->distance(isec.phit));
            if (vis > 0.0f)
            {
                //diffuse
                Vector3 diffuse = isec.color * light->intensity(isec.phit) * cosTheta;

                //specular
                Vector3 toCamera = -ray.direction;
                Vector3 reflected = reflect(-toLight, isec.normal);
                Vector3 specular = isec.material->Ks * powf(std::max(0.0f, toCamera ^ reflected), isec.material->Ns);

                phitColor +=  (diffuse + specular) * vis;
            }
        }
    }
    return phitColor;
}
