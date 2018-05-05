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

        for(auto& light: scene.lights)
        {
            Vector3f toLight = light->direction(isec.phit);

            if (!castShadowRay(Ray(isec.phit + bias * isec.normal, toLight), scene.objects, light->distance(isec.phit)))
            {
                //diffuse NdotL
                float NdotL = std::max(0.0f, isec.normal ^ toLight);
                phitColor += light->intensity(isec.phit) * isec.object->diffColor() * NdotL;

                //specular
                if( NdotL > 0.0f )
                {
                    //specular color
                    Vector3f toCamera = (ray.origin - isec.phit).normalize();
                    Vector3f reflection = (2.0f * ((isec.normal ^ toLight) * isec.normal) - toLight);
                    phitColor += light->intensity(isec.phit) * isec.object->specColor() * pow(std::max(0.0f, toCamera ^ reflection), isec.object->shininess);
                }
            }
        }

        //gamma
        float gamma=1.0/2.2;
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

