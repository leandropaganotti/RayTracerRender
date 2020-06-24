#include "pathtracer.h"
#include "color.h"
#include "material.h"
#include "sphere.h"

const float bias = 0.001f;

Vector3 PathTracer::trace(const Ray &ray, const uint8_t depth, const float E)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;

    if (!castRay(ray, isec))
        return renderOptions.bgColor;

    MaterialType type = isec.material->type;

    if (type == MaterialType::DIFFUSE)
    {
        /*
         * A Lambertian surface by definition reflects radiance equally into all directions. Its BRDF is simply

            $BRDF = ~= rho / M_PI,

            where $ \rho$ is called the reflectivity of the surface (from 0 to 1).

            Lo = E + (BRDF * Li * cos) / pdf; where pdf = 1/Area = 1/2M_PI (hemisphere)

            Lo = E + material->color/M_PI * pathTrace() * cos * 1/pdf
            Lo = E + material->color/M_PI * pathTrace() * cos * 2M_PI
            Lo = E + material->color * pathTrace() * cos * 2
            I am not recurring in case of hit a light so:
            if hit light return E otherwise material->color * pathTrace() * cos * 2
        */
        Ray r;
        r.origin = isec.phit + bias * isec.normal;
        r.direction = randomUnitVectorInHemisphereOf(isec.normal);
        float cosTheta = isec.normal ^ r.direction;
        return isec.material->E + isec.albedo * trace(r, depth+1, E) * cosTheta * 2.0f;
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
            reflected = trace(r, depth+1, 0);
        }
        if (kt > 0.0001f)
        {
            Ray r;
            r.origin = isec.phit + bias * isec.normal;
            r.direction = randomUnitVectorInHemisphereOf(isec.normal);
            float cosTheta = isec.normal ^ r.direction;
            diffused = isec.material->E + isec.albedo * trace(r, depth+1, E) * cosTheta * 2.0f;
        }

        return kt*diffused + kr*reflected;
    }
    else if (type == MaterialType::TRANSPARENT)
    {
        return transparentMaterial(ray, depth, isec, E);
    }
    return color::BLACK;
}

Vector3 PathTracerWithDirectSampling::trace(const Ray &ray, const uint8_t depth, const float E)
{
    if(depth > renderOptions.maxDepth) return color::BLACK;

    IntersectionData isec;

    if (!castRay(ray, isec))
        return renderOptions.bgColor;

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
    }
//    if(isec.material->E != vector::ZERO && depth == 1){
//        float n=5, m=5;
//        float nk = isec.normal ^ -ray.direction;
//        float ek = (n+1)/(2*M_PI)*powf(nk, m);
//        Vector3 Lo = isec.material->E * ek / nk;
//        return Lo;
//    }
    Vector3 diffused, reflected;
    Vector3 brdf = isec.albedo * M_1_PI;
    if (kt > 0.0001f)
    {
        //direct light
        Vector3 direct(0);
//        for(auto &obj : scene->objects)
//        {
//            const GSphere * const sphere = dynamic_cast<const GSphere*const>(obj.get());
//            if (!sphere) continue; // only supported sphere for direct light
//            if (sphere->getMaterial(0)->E  == vector::ZERO) continue; // skip non light

//            Vector3 sampleToLight;
//            float _1_pdf;
//            sphere->sampleSolidAngleSphere(isec.phit, sampleToLight, _1_pdf);

//            float cosTheta = isec.normal ^ sampleToLight;
//            if( cosTheta > 0.0f )
//            {
//                float dist = (isec.phit - sphere->getCenter()).length() - sphere->getRadius() - bias;
//                float vis = castShadowRay(Ray(isec.phit + bias * isec.normal, sampleToLight, dist));
//                if (vis > 0.0f)
//                {
//                    direct += brdf * sphere->getMaterial(0)->E * cosTheta * _1_pdf;
//                }
//            }
//        }

        //indirect light
        const float _1_pdf = (2.0f*M_PI);//1/(2*M_PI)
        Ray r;
        r.origin = isec.phit + bias * isec.normal;
        r.direction = randomUnitVectorInHemisphereOf(isec.normal);
        float cosTheta = isec.normal ^ r.direction;
        //Vector3 indirect =  (E*material->E) + isec.object->color(isec) * pathTracer2(r, scene, depth+1, 0.0f);
        Vector3 indirect =  (E*isec.material->E) + brdf * trace(r, depth+1, 0.0f) * cosTheta * _1_pdf;
        diffused = direct + indirect;

    }

    if(kr > 0.0001f)
    {
        Ray r;
        r.origin = isec.phit + bias * isec.normal;
        r.direction = reflect(ray.direction, isec.normal).normalize();
        reflected = trace(r, depth+1, E);
    }

    return kt*diffused + kr*reflected;
}
