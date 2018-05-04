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
        phitColor = inter.object->c_diffuse * inter.object->k_ambient;

        for(size_t i = 0; i < scene.lights.size(); ++i)
        {
            Vector3f toLight = -scene.lights[i]->direction(inter.phit);

            float distToLight = scene.lights[i]->distance(inter.phit);

            bool inShadow = cast(Ray(inter.phit + bias * inter.normal, toLight), scene.objects, distToLight);

            if (!inShadow)
            {
                //diffuse NdotL
                float NdotL = std::max(0.0f, inter.normal ^ toLight);
                phitColor += scene.lights[i]->intensity(inter.phit) * inter.object->c_diffuse * inter.object->k_diffuse * NdotL;

                //specular
                if( NdotL > 0.0f )
                {
                    //specular color
                    Vector3f toOrigin = (ray.origin - inter.phit).normalize();
                    Vector3f reflection = (2.0f * ((inter.normal ^ toLight) * inter.normal) - toLight);
                    phitColor += scene.lights[i]->intensity(inter.phit) *
                            inter.object->c_specular * inter.object->k_specular *
                            pow(std::max(0.0f, toOrigin ^ reflection), inter.object->shininess);
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
            if ( inter2.dist < closestDist)
            {
                closestDist = inter2.dist;
                inter = inter2; // copy
            }
        }
    }

}

bool RayCaster::cast(const Ray &ray, const ObjectVector &objects, float distToLight)
{
    float dist;
    for (size_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->intersection(ray, dist))
        {
            if ( dist < distToLight )
            {
                return true;
            }
        }
    }
    return false;
}

