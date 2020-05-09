#include "raytracer.h"
#include <typeinfo>
#include <thread>
#include <random>
#include <atomic>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "consts.h"
#include "material.h"
#include "sphere.h"

const float bias = 0.001f;

RayTracer::RayTracer()
{
    setTracer(RayTracerType::Phong);
}

void RayTracer::render(const Scene& scene)
{
    const float grid = scene.grid;
    const float gridSize = 1.0f/grid;
    const size_t nrays_persubpixel = ceilf(scene.spp/(grid*grid));
    const size_t nrays_perpixel = nrays_persubpixel * grid * grid;

    int count = 0;

    buffer.resize(camera.getWidth(), camera.getHeight());
    setTracer(scene.raytracer);

    std::cout << std::endl;
    std::cout << "\r -> 0.00% completed" << std::flush;
    #pragma omp parallel for schedule(dynamic, 1) shared(count)
    for (size_t i = 0; i < camera.getHeight(); ++i)
    {
        std::ostringstream ss;
        for (size_t j = 0; j < camera.getWidth(); ++j)
        {
            buffer.at(i, j) = 0;
            for (size_t ii=0; ii < grid; ++ii)
            {
                unsigned short Xi[3]={0,0,(short unsigned int)(ii)};
                for (size_t jj=0; jj < grid; ++jj)
                {
                    for (size_t n = 0; n < nrays_persubpixel; ++n)
                    {                        
                        const float x = i + gridSize * (ii + erand48(Xi));
                        const float y = j + gridSize * (jj + erand48(Xi));
                        Ray ray(camera.getPosition(), rayDirection(x, y));
                        buffer.at(i, j) += tracer(ray, scene, 1, 1.0f);
                    }
                }
            }
            buffer.at(i, j) /= nrays_perpixel;
        }
        ++count;
        ss << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(camera.getHeight()) * 100.0f << "% completed";
        std::cout << ss.str() << std::flush;
    }
}

inline
Vector3 RayTracer::rayDirection(float i, float j) const
{
    float Px = (2.0f * ((j) / camera.getWidth()) - 1.0f) * tan(camera.getFov() / 2.0f ) * camera.getRatio();
    float Py = (1.0f - 2.0f * ((i) / camera.getHeight())) * tan(camera.getFov() / 2.0f);
    Vector3 dir = (camera.getCameraToWorld() * Vector3(Px, Py, -1.0f)) - camera.getPosition();
    return dir.normalize();
}

inline
bool RayTracer::castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec)
{
    float tmax = FLT_MAX;
    isec.shape = nullptr;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            tmax = isec.tnear;
            isec.shape = object.get();
        }
    }
    if(!isec.shape) return false;

    isec.shape->fetch(ray, isec);
    return true;
}

inline
float RayTracer::castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax)
{
    for(auto &object : objects)
    {
        if (object->intersection(ray, tMax))
        {
            return 0.0f;
        }
    }
    return 1.0f;
}

void RayTracer::setTracer(RayTracerType type){
    switch (type) {
    case RayTracerType::Phong:
        tracer = std::bind(&RayTracer::rayTracer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        break;
    case RayTracerType::PathTracer:
        tracer = std::bind(&RayTracer::pathTracer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        break;
    case RayTracerType::PathTracerWithDirectLightSampling:
        tracer = std::bind(&RayTracer::pathTracer2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        break;
    default:
        tracer = std::bind(&RayTracer::rayTracer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        break;
    }
}

inline
Vector3 RayTracer::specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = reflect(ray.direction, isec.normal).normalize();
    return tracer(R, scene, depth + 1, 1.0f);
}

inline
Vector3 RayTracer::transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, const float E)
{
    const Vector3 &phit = isec.phit;
    Vector3 normal = isec.normal;
    const Material &material = *isec.material;

    Vector3 transmited(0.0f), reflected(0.0f);

    // transmited
    float n, n1, n2, kr, kt;
    float cosi = normal.dot(ray.direction);
    if ( cosi < 0.0f) // outside surface
    {
        cosi = -cosi;
        n1 = scene.ambientIndex;
        n2 = material.Ni;
    }
    else              // inside surface
    {
        normal = -normal;
        n1 = material.Ni;
        n2 = scene.ambientIndex;
    }

    n = n1 / n2;
    float sint2 = n * n * (1.0f - cosi * cosi);

    if (sint2 > 1.0f) { kr = 1.0f; kt = 0.0f; } // TIR

    //refraction
    else
    {
        // fresnel
        float cost  = sqrt(1.0f - sint2);
        float Rs = (n1 * cosi - n2 * cost) / (n1 * cosi + n2 * cost);
        float Rp = (n2 * cosi - n1 * cost) / (n2 * cosi + n1 * cost);

        kr = (Rs * Rs + Rp * Rp) / 2.0f;
        kt = 1.0f - kr;

        Ray T;
        T.origin = phit - bias * normal;
        T.direction = n * ray.direction + (n * cosi - cost) * normal;
        T.direction.normalize();
        transmited = tracer(T, scene, depth + 1, E) * kt;
    }

    // reflection
    if(kr)
    {
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        reflected = tracer(R, scene, depth + 1, E) * kr;
    }
    return transmited + reflected;
}

//inline
//Vector3 RayTracer::globalIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E)
//{
//    const Material *material = &isec.object->getMaterial();

//    if (material->type == Material::Type::TRANSPARENT)
//        return transparentMaterial(ray, scene, depth, isec, E);
//    else if (material->type == Material::Type::SPECULAR)
//    {
//        float c = 1.0f - (isec.normal ^ -ray.direction);
//        float R0 = material->reflectivity;
//        float R = R0 + (1.0f-R0) * c * c * c * c * c;
//        float T = 1.0f - R;
//        if (!T) return specularReflection(ray, scene, depth, isec, E);
//        return T*globalIllumination(ray, scene, depth, isec, E) + R*specularReflection(ray, scene, depth, isec, E);
//    }

//    // Texture
//    const std::pair<float, float> uv = isec.object->uv(isec.phit, isec.idx);

//    // Texture
//    Vector3 textureColor = isec.object->getTexture()->get(uv.first, uv.second);

//    Vector3 brdf = (material->kd * textureColor) / M_PI;

//    //direct light
//    Vector3 directLigthing(0.0f),indirectLigthing(0.0f);

//    if( scene.raytracer == RayTracerType::PathTracerWithDirectLightSampling)
//    for(auto &obj : scene.objects)
//    {
//        if (obj->getMaterial()->Le == Vector::ZERO) continue; // skip non light

//        Sphere *sphere = dynamic_cast<Sphere*>(obj.get());

//        if (!sphere) continue; // only supported sphere for direct light

//        Vector3 u,v, w=(sphere->getCenter() - isec.phit).normalize(), n(1,0,0),m(0,1,0);
//        u = w%n; if(u.length()<0.01f)u = w%m;
//        v=w%u;

//        float r1 = dis(gen);
//        float r2 = dis(gen);
//        float dist2 = (isec.phit - sphere->getCenter()).length(); dist2*=dist2;
//        float cos_a_max = sqrt(1 - (sphere->getRadius()*sphere->getRadius())/dist2);
//        float cos_a = 1 - r2*(1-cos_a_max);
//        float sin_a = sqrtf(1-cos_a*cos_a);
//        float phi = 2 * M_PI * r1;
//        float pdf = 1/(2*M_PI*(1-cos_a_max));

//        Vector3 sample(cos(phi)*sin_a, sin(phi)*sin_a, cos_a);

//        Vector3 sampleWorld = sample.x*u + sample.y*v + sample.z*w;

//        float cosTheta = isec.normal ^ sampleWorld;
//        if( cosTheta > 0.0f )
//        {
//            float dist = (isec.phit - sphere->getCenter()).length() - sphere->getRadius() - bias;
//            float vis = castShadowRay(Ray(isec.phit + bias * isec.normal, sampleWorld), scene.objects, dist);
//            if (vis)
//            {
//                directLigthing += vis * brdf * sphere->getMaterial()->Le * cosTheta / pdf;
//            }
//        }
//    }

//    //indirect light
//    Vector3 u,v, w=isec.normal, n(1,0,0),m(0,1,0);
//    u = w%n; if(u.length()<0.01f)u = w%m;
//    v=w%u;

//    float r1 = dis(gen); // this is cosi
//    float r2 = dis(gen);

//    Vector3 sample = uniformSampleHemisphere(r1, r2);
//    Vector3 sampleWorld = sample.x*u + sample.y*v + sample.z*w;

//    Ray R;
//    R.origin = isec.phit + bias * isec.normal;
//    R.direction = sampleWorld;
//    indirectLigthing = E * material->Le + material->kd * textureColor * oldTracer(R, scene, depth+1, scene.raytracer == RayTracerType::PathTracerWithDirectLightSampling ? 0.0f : 1.0f);

//    return directLigthing + indirectLigthing;
//}

Vector3 RayTracer::rayTracer(const Ray &ray, const Scene &scene, const uint8_t depth, const float)
{
    if(depth > scene.maxDepth) return Color::BLACK;

    IntersectionData isec;

    if (!castRay(ray, scene.objects, isec))
        return scene.bgColor;

    const Material::Type type = isec.material->type;

    if (type == Material::Type::DIFFUSE)
    {
        return phongShading(ray, scene, isec);
    }
    else if (type == Material::Type::SPECULAR)
    {
        float kr = schlick(-ray.direction, isec.normal, isec.material->R0);
        float kt = 1.0f - kr;
        Vector3 diffused(0), reflected(0);
        if (kr > 0.0001f)
        {
            Ray r;
            r.origin = isec.phit + bias * isec.normal;
            r.direction = reflect(ray.direction, isec.normal).normalize();
            reflected = rayTracer(r, scene, depth + 1);
        }
        if (kt > 0.0001f)
            diffused = phongShading(ray, scene, isec);

        return kt*diffused + kr*reflected;
    }
    else if (type == Material::Type::TRANSPARENT)
    {
        return transparentMaterial(ray, scene, depth, isec);
    }
    return Color::BLACK;
}

inline
Vector3 RayTracer::pathTracer(const Ray &ray, const Scene &scene, const uint8_t depth, const float)
{
    if(depth > scene.maxDepth) return Color::BLACK;

    IntersectionData isec;

    if (!castRay(ray, scene.objects, isec))
        return scene.bgColor;

    Material::Type type = isec.material->type;

    if (type == Material::Type::DIFFUSE)
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
        return isec.material->E + isec.color * pathTracer(r, scene, depth+1) * cosTheta * 2.0f;
    }
    else if (type == Material::Type::SPECULAR)
    {
        float kr = schlick(-ray.direction, isec.normal, isec.material->R0);
        float kt = 1.0f - kr;
        Vector3 diffused(0), reflected(0);
        if (kr > 0.0001)
        {
            Ray r;
            r.origin = isec.phit + bias * isec.normal;
            r.direction = reflect(ray.direction, isec.normal).normalize();
            reflected = pathTracer(r, scene, depth+1);
        }
        if (kt > 0.0001)
        {
            Ray r;
            r.origin = isec.phit + bias * isec.normal;
            r.direction = randomUnitVectorInHemisphereOf(isec.normal);
            float cosTheta = isec.normal ^ r.direction;
            diffused = isec.material->E + isec.color * pathTracer(r, scene, depth+1) * cosTheta * 2.0f;
        }

        return kt*diffused + kr*reflected;
    }
    else if (type == Material::Type::TRANSPARENT)
    {
        return transparentMaterial(ray, scene, depth, isec);
    }
    return Color::BLACK;
}

Vector3 RayTracer::pathTracer2(const Ray& ray, const Scene& scene, const uint8_t depth, const float E)
{
    if(depth > scene.maxDepth) return Color::BLACK;

    IntersectionData isec;

    if (!castRay(ray, scene.objects, isec))
        return Color::BLACK;

    const Material::Type type = isec.material->type;

    float kt=1.0f, kr=0.0f; // default diffuse material values
    if(type == Material::Type::TRANSPARENT)
    {
        return transparentMaterial(ray, scene, depth, isec, E);
    }
    else if (type == Material::Type::SPECULAR)
    {
        kr = schlick(-ray.direction, isec.normal, isec.material->R0);
        kt = 1.0f - kr;
    }
    if(isec.material->E != Vector::ZERO && depth == 1){
        float n=5, m=15;
        float nk = isec.normal ^ -ray.direction;
        float ek = (n+1)/(2*M_PI)*powf(nk, m);

        Vector3 Lo = isec.material->E * ek / nk;

        return Lo;
    }
    Vector3 diffused, reflected;
    Vector3 brdf = isec.color * M_1_PI;
    if (kt > 0.0001f)
    {
        //direct light
        Vector3 direct(0);
        for(auto &obj : scene.objects)
        {
            const GSphere * const sphere = dynamic_cast<const GSphere*const>(obj.get());
            if (!sphere) continue; // only supported sphere for direct light
            if (sphere->getMaterial()->E  == Vector::ZERO) continue; // skip non light

            Vector3 sampleToLight;
            float _1_pdf;
            sphere->sampleSolidAngleSphere(isec.phit, sampleToLight, _1_pdf);

            float cosTheta = isec.normal ^ sampleToLight;
            if( cosTheta > 0.0f )
            {
                float dist = (isec.phit - sphere->getCenter()).length() - sphere->getRadius() - bias;
                float vis = castShadowRay(Ray(isec.phit + bias * isec.normal, sampleToLight), scene.objects, dist);
                if (vis > 0.0f)
                {
                    direct += vis * brdf * sphere->getMaterial()->E * cosTheta * _1_pdf;
                }
            }
        }

        //indirect light
        const float _1_pdf = (2.0f*M_PI);//1/(2*M_PI)
        Ray r;
        r.origin = isec.phit + bias * isec.normal;
        r.direction = randomUnitVectorInHemisphereOf(isec.normal);
        float cosTheta = isec.normal ^ r.direction;
        //Vector3 indirect =  (E*material->E) + isec.object->color(isec) * pathTracer2(r, scene, depth+1, 0.0f);
        Vector3 indirect =  (E*isec.material->E) + brdf * pathTracer2(r, scene, depth+1, 0.0f) * cosTheta * _1_pdf;
        diffused = direct + indirect;

    }

    if(kr > 0.0001f)
    {
        Ray r;
        r.origin = isec.phit + bias * isec.normal;
        r.direction = reflect(ray.direction, isec.normal).normalize();
        reflected = pathTracer2(r, scene, depth+1, E);
    }

    return kt*diffused + kr*reflected;
}


Vector3 RayTracer::phongShading(const Ray &ray, const Scene &scene, const IntersectionData &isec)
{
    //ambient
    Vector3 phitColor = isec.color * scene.ka;

    for(auto& light: scene.lights)
    {
        Vector3 toLight = light->direction(isec.phit);

        float cosTheta = isec.normal ^ toLight; //NdotL
        if( cosTheta > 0.0f )
        {
            float vis = castShadowRay(Ray(isec.phit + bias * isec.normal, toLight), scene.objects, light->distance(isec.phit));
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
