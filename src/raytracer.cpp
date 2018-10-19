#include "raytracer.h"
#include <thread>
#include <random>
#include <atomic>
#include <iomanip>
#include <sstream>
#include "sphere.h"
#include <iostream>
#include "consts.h"

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<float> dis(0, 1);
const float bias = 0.001;

inline
bool RayTracer::closestIntersection(const Ray &ray, const ObjectVector &objects, IntersectionData &isec)
{
    IntersectionData isec_tmp;
    isec.object = nullptr;
    isec.tnear  = FLT_MAX;

    for(auto &object : objects)
    {
        if (object->intersection(ray, isec_tmp))
        {
            if ( isec_tmp.tnear < isec.tnear )
            {
                isec = isec_tmp; // copy
            }
        }
    }
    if(isec.object)
    {
        isec.phit = ray.origin + isec.tnear * ray.direction;
        isec.normal = isec.object->normal(isec.phit, isec.idx);
        return true;
    }
    return false;
}

inline
float RayTracer::castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax)
{
    float tnear;
    float vis = 1.0f;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tnear))
        {
            if ( tnear < tMax )
            {
                if (object->material.type != Material::Type::TRANSPARENT)
                    return 0.0f;
                else
                {
                    vis -= 0.3f;
                }
            }
        }
    }
    return vis;
}

inline
Vector3 RayTracer::diffusePhongReflection(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec, float)
{
    const Vector3 &phit = isec.phit;
    const Vector3 normal = isec.normal.dot(ray.direction) > 0.0f ? -isec.normal: isec.normal;
    const Material &material = isec.object->material;

    // Texture
    Vector3 texture = isec.object->texture(phit, isec.idx);

    //ambient
    Vector3 phitColor = material.kd * texture * scene.ka;

    for(auto& light: scene.lights)
    {
        Vector3 toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            float vis = castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit));
            if (vis)
            {
                //diffuse
                Vector3 diffuse = material.kd * texture * incidence;

                //specular
                Vector3 toCamera = -ray.direction;
                Vector3 reflected = reflect(-toLight, normal);
                Vector3 specular = material.highlight * pow(std::max(0.0f, toCamera ^ reflected), material.shininess);

                phitColor +=  light->intensity(phit) * (diffuse + specular) * vis;
            }
        }
    }
    return phitColor;
}

inline
Vector3 RayTracer::specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E)
{
    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = reflect(ray.direction, isec.normal).normalize();
    return castRay(R, scene, depth + 1, E) * isec.object->material.ks;
}

inline
Vector3 RayTracer::transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E)
{
    const Vector3 &phit = isec.phit;
    Vector3 normal = isec.normal;
    const Material &material = isec.object->material;

    Vector3 phitColor(0.0f);

    // specular
    for(auto& light: scene.lights)
    {
        Vector3 toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                //specular
                Vector3 toCamera = -ray.direction;
                Vector3 reflected = reflect(-toLight, normal);
                Vector3 specular = material.highlight * pow(std::max(0.0f, toCamera ^ reflected), material.shininess);
                phitColor +=  light->intensity(phit) * specular;
            }
        }
    }

    // transmited
    float n, n1, n2, kr, kt;
    float cosi = normal.dot(ray.direction);
    if ( cosi < 0.0f) // outside surface
    {
        cosi = -cosi;
        n1 = scene.ambientIndex;
        n2 = material.refractiveIndex;
    }
    else              // inside surface
    {
        normal = -normal;
        n1 = material.refractiveIndex;
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
        phitColor += castRay(T, scene, depth + 1, E) * kt;
    }


    // reflection
    if(kr)
    {
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        phitColor += castRay(R, scene, depth + 1, E) * kr;
    }
    return phitColor;
}

void RayTracer::capture(const Scene &scene)
{
    const float grid = scene.grid;
    const float gridSize = 1.0f/grid;

    const size_t nrays = roundf(scene.spp/(grid*grid));

    int count = 0;

    std::cout << std::endl;
    std::cout << "\r -> 0.00% completed" << std::flush;
    #pragma omp parallel for schedule(dynamic, 1) shared(count)
    for (size_t i = 0; i < getHeight(); ++i)
    {
        std::ostringstream ss;
        for (size_t j = 0; j < getWidth(); ++j)
        {
            image.at(i, j) = 0;
            for (size_t ii=0; ii < grid; ++ii)
            {
                unsigned short Xi[3]={0,0,(short unsigned int)(ii*ii*ii)};
                for (size_t jj=0; jj < grid; ++jj)
                {
                    for (size_t n = 0; n < nrays; ++n)
                    {
                        float r1 = erand48(Xi) * gridSize;
                        float r2 = erand48(Xi) * gridSize;
                        Ray ray(getPosition(), getDirection(i + gridSize*ii + r1, j + gridSize*jj + r2));
                        image.at(i, j) += castRay(ray, scene, 1);
                    }
                }
            }
            image.at(i, j) /= nrays*grid*grid;
        }
        ++count;
        ss << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(getHeight()) * 100.0f << "% completed";
        std::cout << ss.str() << std::flush;
    }
}

inline
Vector3 RayTracer::getDirection(float i, float j) const
{
    float Px = (2.0f * ((j) / getWidth()) - 1.0f) * tan(getFov() / 2.0f ) * getRatio();
    float Py = (1.0f - 2.0f * ((i) / getHeight())) * tan(getFov() / 2.0f);
    Vector3 dir = (cameraToWorld * Vector3(Px, Py, -1.0f)) - getPosition();
    return dir.normalize();
}

inline
Vector3 RayTracer::castRay(const Ray &ray, const Scene &scene, const uint8_t depth, float E)
{
    if(depth > scene.maxDepth) return Color::BLACK;

    IntersectionData isec;

    if (!closestIntersection(ray, scene.objects, isec))
        return scene.bgColor;

    if (scene.shader == Shader::PHONG)
    {
        return phongIllumination(ray, scene, depth, isec, E);
    }
    else if (scene.shader == Shader::GI || scene.shader == Shader::GI_DIRECT)
    {
        return globalIllumination(ray, scene, depth, isec, E);
    }
    else
    {
        std::cerr << "Shader not recognized" << std::endl;
        exit(0);
    }
    return Vector::ZERO;
}

inline
Vector3 RayTracer::phongIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float)
{
    Material::Type type = isec.object->material.type;

    if (type == Material::Type::DIFFUSE)
    {
        return diffusePhongReflection(ray, scene, depth, isec);
    }
    else if (type == Material::Type::SPECULAR)
    {
        float c = 1.0f - (isec.normal ^ -ray.direction);
        float R0 = isec.object->material.reflectivity;
        float R = R0 + (1.0f-R0) * c * c * c * c * c;
        float T = 1.0f - R;
        if (!T)

            return specularReflection(ray, scene, depth, isec);
        else
        {
            return T*diffusePhongReflection(ray, scene, depth, isec) + R*specularReflection(ray, scene, depth, isec);
        }
    }
    else if (type == Material::Type::TRANSPARENT)
    {
        return transparentMaterial(ray, scene, depth, isec);
    }
    return Vector::ZERO;
}

inline
Vector3 RayTracer::globalIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E)
{    
    const Material *material = &isec.object->material;

    if (material->type == Material::Type::TRANSPARENT)
        return transparentMaterial(ray, scene, depth, isec, E);
    else if (material->type == Material::Type::SPECULAR)
    {
        float c = 1.0f - (isec.normal ^ -ray.direction);
        float R0 = material->reflectivity;
        float R = R0 + (1.0f-R0) * c * c * c * c * c;
        float T = 1.0f - R;
        if (!T) return specularReflection(ray, scene, depth, isec, E);
        return T*globalIllumination(ray, scene, depth, isec, E) + R*specularReflection(ray, scene, depth, isec, E);
    }

    // Texture
    Vector3 textureColor = isec.object->texture(isec.phit, isec.idx);

    Vector3 brdf = (material->kd * textureColor) / M_PI;

    //direct light
    Vector3 directLigthing(0.0f),indirectLigthing(0.0f);

    if( scene.shader == Shader::GI_DIRECT)
    for(auto &obj : scene.objects)
    {
        if (obj->material.Le == Vector::ZERO) continue; // skip non light

        //if (typeid(obj) != typeid(Sphere)) continue;    // only sphere sampling

        Sphere *sphere = dynamic_cast<Sphere*>(obj.get());
        if (isec.object == sphere) continue;

        Vector3 u,v, w=(sphere->getCenter() - isec.phit).normalize(), n(1,0,0),m(0,1,0);
        u = w%n; if(u.length()<0.01f)u = w%m;
        v=w%u;

        float r1 = dis(gen);
        float r2 = dis(gen);
        float dist2 = (isec.phit - sphere->getCenter()).length(); dist2*=dist2;
        float cos_a_max = sqrt(1 - (sphere->getRadius()*sphere->getRadius())/dist2);
        float cos_a = 1 - r2*(1-cos_a_max);
        float sin_a = sqrtf(1-cos_a*cos_a);
        float phi = 2 * M_PI * r1;
        float pdf = 1/(2*M_PI*(1-cos_a_max));

        Vector3 sample(cos(phi)*sin_a, sin(phi)*sin_a, cos_a);

        Vector3 sampleWorld = sample.x*u + sample.y*v + sample.z*w;

        float cosTheta = isec.normal ^ sampleWorld;
        if( cosTheta > 0.0f )
        {
            float dist = (isec.phit - sphere->getCenter()).length() - sphere->getRadius() - bias;
            float vis = castShadowRay(Ray(isec.phit + bias * isec.normal, sampleWorld), scene.objects, dist);
            if (vis)
            {
                directLigthing += vis * brdf * sphere->material.Le * cosTheta / pdf;
            }
        }
    }        

    //indirect light    
    Vector3 u,v, w=isec.normal, n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;

    float r1 = dis(gen); // this is cosi
    float r2 = dis(gen);

    Vector3 sample = uniformSampleHemisphere(r1, r2);
    Vector3 sampleWorld = sample.x*u + sample.y*v + sample.z*w;

    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = sampleWorld;
    indirectLigthing = E * material->Le + material->kd * textureColor * castRay(R, scene, depth+1, scene.shader == Shader::GI_DIRECT ? 0.0f : 1.0f);

    return directLigthing + indirectLigthing;
}