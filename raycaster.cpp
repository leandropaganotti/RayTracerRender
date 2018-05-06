#include "raycaster.h"
#include "float.h"

Vector3f RayCaster::cast(const Ray &ray, const Scene &scene)
{
    IntersectionData isec;

    cast(ray, scene.objects, isec);

    if (isec.object)
    {
        float bias = 0.0001;

        Vector3f phitColor(0);

        //ambient color
        phitColor = isec.object->ambient();

        Vector3f phit = ray.origin + isec.tnear * ray.direction;
        Vector3f normal = isec.object->normal(phit, isec.idx);

        for(auto& light: scene.lights)
        {
            Vector3f toLight = light->direction(phit);

            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                //diffuse NdotL
                float NdotL = std::max(0.0f, normal ^ toLight);
                phitColor += light->intensity(phit) * isec.object->diffColor() * NdotL;

                //specular
                if( NdotL > 0.0f )
                {
                    //specular color
                    Vector3f toCamera = (ray.origin - phit).normalize();
                    Vector3f reflection = (2.0f * ((normal ^ toLight) * normal) - toLight);
                    phitColor += light->intensity(phit) * isec.object->specColor() * pow(std::max(0.0f, toCamera ^ reflection), isec.object->shininess);
                }
            }
        }

        //gamma
        const float gamma=1.0/2.2;
        phitColor[0] = pow(phitColor[0], gamma);
        phitColor[1] = pow(phitColor[1], gamma);
        phitColor[2] = pow(phitColor[2], gamma);

        return phitColor;
    }
    return scene.bgColor;
}

void RayCaster::cast(const Ray &ray, const ObjectVector &objects, IntersectionData &isec)
{
    IntersectionData isec_tmp;

    float tnear = FLT_MAX;

    isec.object = nullptr;

    for(auto &object : objects)
    {
        if (object->intersection(ray, isec_tmp))
        {
            if ( isec_tmp.tnear < tnear)
            {
                tnear = isec_tmp.tnear;
                isec = isec_tmp; // copy
            }
        }
    }
}

bool RayCaster::castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax)
{
    float tnear;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tnear))
        {
            if ( tnear > 0 && tnear < tMax ) return true;
        }
    }
    return false;
}

