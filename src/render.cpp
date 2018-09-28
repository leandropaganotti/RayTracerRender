#include "render.h"
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

const Image &Render::getImage() const
{
    return image;
}

inline
bool Render::castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec)
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
bool Render::castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax)
{
    float tnear;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tnear))
        {
            if ( tnear < tMax ) return true;
        }
    }
    return false;
}

inline
Vector3 Render::phongReflection(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec)
{
    const Vector3 &phit = isec.phit;
    const Vector3 normal = isec.normal.dot(ray.direction) > 0.0f ? -isec.normal: isec.normal;
    const Material &material = isec.object->material;

    // Texture
    Vector3 texture = isec.object->texture(phit, isec.idx);

    //ambient
    Vector3 phitColor = material.kd * 1 * scene.ka;

    for(auto& light: scene.lights)
    {
        Vector3 toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                //diffuse
                Vector3 diffuse = material.kd * texture * incidence;

                //specular
                Vector3 toCamera = -ray.direction;
                Vector3 reflected = reflect(-toLight, normal);
                Vector3 specular = material.highlight * pow(std::max(0.0f, toCamera ^ reflected), material.shininess);

                phitColor +=  light->intensity(phit) * (diffuse + specular);
            }
        }
    }
    return phitColor;
}

inline
Vector3 Render::specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = reflect(ray.direction, isec.normal).normalize();
    return trace(R, scene, depth + 1) * isec.object->material.ks;
}

inline
Vector3 Render::transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
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
        phitColor += trace(T, scene, depth + 1) * kt;
    }


    // reflection
    if(kr)
    {
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        phitColor += trace(R, scene, depth + 1) * kr;
    }
    return phitColor;
}

void Render::render(const Scene &scene)
{
    const float grid = scene.grid;
    const float gridSize = 1.0f/grid;

    const size_t nrays = roundf(scene.spp/(grid*grid));

    int count = 0;

    std::cout << std::endl;
    std::cout << "\r -> 0.00% completed" << std::flush;
    #pragma omp parallel for schedule(dynamic, 1) shared(count)
    for (size_t i = 0; i < options.height; ++i)
    {
        std::ostringstream ss;
        for (size_t j = 0; j < options.width; ++j)
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
                        Ray ray(options.from, getRayDirection(i + gridSize*ii + r1, j + gridSize*jj + r2));
                        image.at(i, j) += trace(ray, scene, 1);
                    }
                }
            }
            image.at(i, j) /= nrays*grid*grid;
        }
        ++count;
        ss << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(options.height) * 100.0f << "% completed";
        std::cout << ss.str() << std::flush;
    }
}

Vector3 Render::trace(const Ray &ray, const Scene &scene, const uint8_t depth)
{
    if(depth > scene.maxDepth) return Vector3(0.0f);

    IntersectionData isec;

    if (!castRay(ray, scene.objects, isec))
        return scene.bgColor;

    Material::Type type = isec.object->material.type;

    if (type == Material::Type::DIFFUSE)
    {
        if (scene.shade == Shade::PHONG)
            return phongReflection(ray, scene, depth, isec);
        else
            return pathTrace(ray, scene, depth, isec);
    }
    else if (type == Material::Type::SPECULAR)
    {
        float c = 1 - (isec.normal ^ -ray.direction);
        float R0 = isec.object->material.reflectivity;
        float R = R0 + (1-R0) * c * c * c * c * c;
        float T = 1-R;
        return T*phongReflection(ray, scene, depth, isec) + R*specularReflection(ray, scene, depth, isec);
    }
    else if (type == Material::Type::TRANSPARENT)
    {
        return transparentMaterial(ray, scene, depth, isec);
    }    
    else
    {
        return Vector3(0.0f);
    }
}

inline
Vector3 Render::pathTrace(const Ray &, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    const Material *material = &isec.object->material;

    if (material->Le.x != 0 || material->Le.y != 0 || material->Le.z != 0)
    {
        return material->Le;
        /* this here for better direct sample */
//        float max = material->Le.x>material->Le.y &&
//                    material->Le.x>material->Le.z ? material->Le.x :
//                    material->Le.y>material->Le.z ? material->Le.y :
//                    material->Le.z; // max refl
//        return material->Le/max;
    }
    // Texture
    Vector3 textureColor = isec.object->texture(isec.phit, isec.idx);

    Vector3 brdf = (material->kd * textureColor) / M_PI;

    //direct light
    Vector3 directLigthing(0.0f),indirectLigthing(0.0f), Nt, Nb;

    if( scene.shade == Shade::GI_DIRECT)
    for(auto &obj : scene.objects)
    {
        if (obj->material.Le.x == 0 && obj->material.Le.y == 0 && obj->material.Le.z == 0) continue; // skip non light

        Sphere *sphere = dynamic_cast<Sphere*>(obj.get());
        Vector3 N = (sphere->getCenter() - isec.phit).normalize();
        createCoordinateSystem(N, Nt, Nb);

        float r1 = dis(gen);
        float r2 = dis(gen);
        float dist2 = (isec.phit - sphere->getCenter()).length(); dist2*=dist2;
        float cos_a_max = sqrt(1 - (sphere->getRadius()*sphere->getRadius())/dist2);
        float cos_a = 1 - r2*(1-cos_a_max);
        float sin_a = sqrtf(1-cos_a*cos_a);
        float phi = 2 * M_PI * r1;
        float pdf = 1/(2*M_PI*(1-cos_a_max));

        Vector3 sample(cos(phi)*sin_a, cos_a, sin(phi)*sin_a);
        Vector3 toLight(
                sample.x * Nb.x + sample.y * N.x + sample.z * Nt.x,
                sample.x * Nb.y + sample.y * N.y + sample.z * Nt.y,
                sample.x * Nb.z + sample.y * N.z + sample.z * Nt.z);

        float cosTheta = isec.normal ^ toLight;
        if( cosTheta > 0.0f )
        {
            float dist = (isec.phit - sphere->getCenter()).length() - sphere->getRadius() - bias;
            if (!castShadowRay(Ray(isec.phit + bias * isec.normal, toLight), scene.objects, dist))
            {
                directLigthing += brdf * sphere->material.Le * cosTheta / pdf;
            }
        }
    }

    //indirect light
    createCoordinateSystem(isec.normal, Nt, Nb);
    float r1 = dis(gen); // this is cosi
    float r2 = dis(gen);
    Vector3 sample = uniformSampleHemisphere(r1, r2);
    float pdf = 1 / (2 * M_PI);
    Vector3 sampleWorld(
            sample.x * Nb.x + sample.y * isec.normal.x + sample.z * Nt.x,
            sample.x * Nb.y + sample.y * isec.normal.y + sample.z * Nt.y,
            sample.x * Nb.z + sample.y * isec.normal.z + sample.z * Nt.z);

    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = sampleWorld;
    indirectLigthing = brdf * trace(R, scene, depth + 1) * r1 / pdf;

    if(directLigthing.x>material->kd.x) directLigthing.x=material->kd.x;
    if(directLigthing.y>material->kd.y) directLigthing.y=material->kd.y;
    if(directLigthing.z>material->kd.z) directLigthing.z=material->kd.z;

    return indirectLigthing + directLigthing;
}
