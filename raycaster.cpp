#include "raycaster.h"
#include "float.h"


RayCaster::RayCaster()
{

}

Vector3f RayCaster::cast(const Ray &ray, const Scene &scene)
{
    IntersectionData inter;

    cast(ray, scene.objects, inter);

    if (inter.object)
    {
        float bias = 0.0001;

        Vector3f phitColor(0);

        //ambient color
        phitColor = inter.object->ambient();

        for (Light *light : scene.lights)
        {
            Vector3f toLight = light->direction(inter.phit);

            if (!castShadowRay(Ray(inter.phit + bias * inter.normal, toLight), scene.objects, light->distance(inter.phit)))
            {
                //diffuse NdotL
                float NdotL = std::max(0.0f, inter.normal ^ toLight);
                phitColor += light->intensity(inter.phit) * inter.object->diffColor() * NdotL;

                //specular
                if( NdotL > 0.0f )
                {
                    //specular color
                    Vector3f toCamera = (ray.origin - inter.phit).normalize();
                    Vector3f reflection = (2.0f * ((inter.normal ^ toLight) * inter.normal) - toLight);
                    phitColor += light->intensity(inter.phit) * inter.object->specColor() * pow(std::max(0.0f, toCamera ^ reflection), inter.object->shininess);
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
    return scene.backgroundColor;
}

void RayCaster::cast(const Ray &ray, const ObjectVector &objects, IntersectionData &inter)
{
    IntersectionData inter2;

    float closestDist = FLT_MAX;

    inter.object = nullptr;

    for(size_t i=0; i < objects.size() ; ++i)
    {
        if (objects[i]->intersection(ray, inter2))
        {
            if ( inter2.tNear < closestDist)
            {
                closestDist = inter2.tNear;
                inter = inter2; // copy
            }
        }
    }

}

bool RayCaster::castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax)
{
    float tNear;
    for (size_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->intersection(ray, tNear))
        {
            if ( tNear > 0 && tNear < tMax )
            {
                return true;
            }
        }
    }
    return false;
}

