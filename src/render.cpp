#include "render.h"
#include <thread>
#include <random>
#include <atomic>
#include <iomanip>
#include <sstream>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

const Image &Render::getImage() const
{
    return image;
}

Vector3f Render::rayTrace(const Ray &ray, const Scene &scene, const uint8_t depth)
{
    if(depth > scene.maxDepth) return Vector3f(0.0f);

    IntersectionData isec;

    if (!castRay(ray, scene.objects, isec))
        return scene.bgcolor;

    switch (isec.object->material.type) {
    case Material::Type::DIFFUSE:
        return diffuseReflection(ray, scene, depth, isec);
    case Material::Type::SPECULAR:
        return specularMaterial(ray, scene, depth, isec);
    case Material::Type::MIRROR:
        return mirrorMaterial(ray, scene, depth, isec);
    case Material::Type::TRANSPARENT:
        return transparentMaterial(ray, scene, depth, isec);
    default:
        return Vector3f(0.0f);
    }
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
Vector3f Render::diffuseMaterial(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec)
{
    const Vector3f &phit = isec.phit;
    const Vector3f normal = isec.normal.dot(ray.direction) > 0.0f ? -isec.normal: isec.normal;
    const Material &material = isec.object->material;

    // Texture
    Vector3f textureColor = isec.object->texture(phit, isec.idx);

    //ambient
    Vector3f phitColor = material.kDiffuse * textureColor * scene.kAmbient;

    for(auto& light: scene.lights)
    {
        Vector3f toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                //diffuse
                Vector3f diffuse = material.kDiffuse * textureColor * incidence;

                //specular
                Vector3f toCamera = -ray.direction;
                Vector3f reflected = reflect(-toLight, normal);
                Vector3f specular = material.specularHighlight * pow(std::max(0.0f, toCamera ^ reflected), material.shininess);

                phitColor +=  light->intensity(phit) * (diffuse + specular);
            }
        }
    }
    return phitColor;
}

inline
Vector3f Render::specularMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = reflect(ray.direction, isec.normal).normalize();

    Vector3f phitColor(0.0f);
    phitColor = diffuseReflection(ray, scene, depth, isec) * (1.0f - isec.object->material.reflectivity);

    return phitColor + rayTrace(R, scene, depth + 1) * isec.object->material.kSpecular * isec.object->material.reflectivity;
}

inline
Vector3f Render::mirrorMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = reflect(ray.direction, isec.normal).normalize();
    return rayTrace(R, scene, depth + 1) * isec.object->material.kSpecular;
}

inline
Vector3f Render::transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    const Vector3f &phit = isec.phit;
    Vector3f normal = isec.normal;
    const Material &material = isec.object->material;

    Vector3f phitColor(0.0f);

    // specular
    for(auto& light: scene.lights)
    {
        Vector3f toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                //specular
                Vector3f toCamera = -ray.direction;
                Vector3f reflected = reflect(-toLight, normal);
                Vector3f specular = material.specularHighlight * pow(std::max(0.0f, toCamera ^ reflected), material.shininess);
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
        phitColor += rayTrace(T, scene, depth + 1) * kt;
    }


    // reflection
    if(kr)
    {
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        phitColor += rayTrace(R, scene, depth + 1) * kr;
    }
    return phitColor;
}

void Render::render(const Scene &scene)
{
    size_t nthreads = options.getHeight();

    std::vector<std::thread> threads;

    size_t nrows = options.getHeight() / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( &Render::renderSingleThread, this, &scene, i*nrows, (i+1) * nrows) );
    }
    threads.push_back( std::thread( &Render::renderSingleThread, this, &scene, i*nrows, options.getHeight()) );

    for (auto& thread : threads) thread.join();
}

void Render::render_omp(const Scene &scene)
{
    const float grid = 1.0f / scene.nprays;

    int count = 0;

    std::cout << std::endl;
    std::cout << "\r -> 0.00% completed" << std::flush;

    #pragma omp parallel for schedule(dynamic, 1) shared(count)
    for (size_t i = 0; i < options.height; ++i)
    {
        std::ostringstream stdStream;
        for (size_t j = 0; j < options.width; ++j)
        {
            image.at(i, j) = 0;
            for (size_t y=0; y < scene.nprays; ++y)
            {
                unsigned short Xi[3]={0,0,y*y*y};
                for (size_t x=0; x < scene.nprays; ++x)
                {
                    float randX = erand48(Xi) * grid;
                    float randY = erand48(Xi) * grid;
                    Ray ray(options.from, getRayDirection(i + grid*y + randY, j + grid*x + randX));
                    image.at(i, j) += rayTrace(ray, scene, 1);
                }
            }
            image.at(i, j) /= scene.nprays*scene.nprays;
        }
        ++count;
        stdStream << "\r -> " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) << count/float(options.height) * 100.0f << "% completed";
        std::cout << stdStream.str() << std::flush;
    }
}

void Render::renderSingleThread(const Scene *scene, size_t startRow, size_t endRow)
{
    if (scene == nullptr) return;

    Ray ray;
    ray.origin = options.from;

    // nrays defines a grid nrays x nrays
    float dx = 1.0f / ( 1.0f + scene->nprays);
    float dy = 1.0f / ( 1.0f + scene->nprays);

    for (size_t i = startRow; i < endRow; ++i)
    {
        for (size_t j = 0; j < options.width; ++j)
        {
            image.at(i, j) = 0;
            for (size_t y=1; y <= scene->nprays; ++y)
            {
                for (size_t x=1; x <= scene->nprays; ++x)
                {
                    ray.direction = getRayDirection(i+dy*y, j+dx*x);
                    image.at(i, j) += rayTrace(ray, *scene, 1);
                }
            }
            image.at(i, j) /= scene->nprays*scene->nprays;
        }
    }
}

Vector3f Render::diffuseReflection(const Ray &, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    const Material *material = &isec.object->material;

    if (material->emission.x != 0 || material->emission.y != 0 || material->emission.z != 0)
        return material->emission;

    Vector3f indirectLigthing(0.0f);
    Vector3f Nt, Nb;

    createCoordinateSystem(isec.normal, Nt, Nb);

    float r1 = distribution(generator); // this is cosi
    float r2 = distribution(generator);
    Vector3f sample = uniformSampleHemisphere(r1, r2);
    Vector3f sampleWorld(
            sample.x * Nb.x + sample.y * isec.normal.x + sample.z * Nt.x,
            sample.x * Nb.y + sample.y * isec.normal.y + sample.z * Nt.y,
            sample.x * Nb.z + sample.y * isec.normal.z + sample.z * Nt.z);

    Ray R;
    R.origin = isec.phit + bias * isec.normal;
    R.direction = sampleWorld;
    indirectLigthing += r1 * rayTrace(R, scene, depth + 1) ;

    // Texture
    Vector3f textureColor = isec.object->texture(isec.phit, isec.idx);

    return material->emission + (2 * indirectLigthing) * material->kDiffuse * textureColor;
}
