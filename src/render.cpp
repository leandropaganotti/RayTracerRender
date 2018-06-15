#include "render.h"
#include <thread>

void Render::setCameraOptions(const CameraOptions &options)
{
    setOptions(options);
}

Camera &Render::getCamera()
{
    return *this;
}

const Image &Render::getImage() const
{
    return image;
}

void Render::render(size_t width, size_t height, const Scene &scene, uint8_t nrays, uint8_t nthreads)
{
   setResolution(width, height);
   render(scene, nrays, nthreads);
}

void Render::render(const CameraOptions &opts, const Scene &scene, uint8_t nrays, uint8_t nthreads)
{
    setCameraOptions(opts);
    render(scene, nrays, nthreads);
}

void Render::renderSingleThread(const Scene *scene, size_t startRow, size_t endRow, size_t nrays)
{
    if (scene == nullptr) return;

    Ray ray;
    ray.origin = options.from;

    // nrays defines a grid nrays x nrays
    float dx = 1.0f / ( 1.0f + nrays);
    float dy = 1.0f / ( 1.0f + nrays);

    for (size_t i = startRow; i < endRow; ++i)
    {
        for (size_t j = 0; j < options.width; ++j)
        {
            image.at(i, j) = 0;
            for (size_t y=1; y <= nrays; ++y)
            {
                for (size_t x=1; x <= nrays; ++x)
                {
                    ray.direction = getRayDirection(i+dy*y, j+dx*x);
                    image.at(i, j) += rayTrace(ray, *scene, 0);
                }
            }
            image.at(i, j) /= nrays*nrays;
        }
    }
}

Vector3f Render::rayTrace(const Ray &ray, const Scene &scene, const uint8_t depth)
{
    if(depth == MAX_DEPTH) return Vector3f(0.0f);

    IntersectionData isec;

    castRay(ray, scene.objects, isec);

    if (isec.object == nullptr) return Vector3f(0.0f);

    switch (isec.object->material.type) {
    case Material::Type::DIFFUSE:
        return diffuseMaterial(ray, scene, depth, isec);
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
void Render::castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec)
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
}

inline
bool Render::castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax)
{
    float tnear;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tnear))
        {
            if ( tnear < tMax && object->material.type != Material::Type::TRANSPARENT) return true;
            //if ( tnear < tMax ) return true;
        }
    }
    return false;
}

inline
Vector3f Render::diffuseMaterial(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec)
{
    Vector3f phit = ray.origin + isec.tnear * ray.direction;
    Vector3f normal = isec.object->normal(phit, isec.idx);
    const Material *material = &isec.object->material;

    // Texture
    Vector3f textureColor = isec.object->texture(phit, isec.idx);

    //ambient
    Vector3f phitColor = material->kDiffuse * textureColor * scene.kAmbient;

    // if inside surface, inverting normal direction
    float cosi = normal.dot(ray.direction);
    if ( cosi > 0.0f) // inside surface
        normal = -normal;

    // diffuse
    for(auto& light: scene.lights)
    {
        Vector3f toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                phitColor +=  light->intensity(phit) * (material->kDiffuse * textureColor * incidence);
            }
        }
    }
    return phitColor;
}

inline
Vector3f Render::specularMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Vector3f phit = ray.origin + isec.tnear * ray.direction;
    Vector3f normal = isec.object->normal(phit, isec.idx);
    const Material *material = &isec.object->material;

    // Texture
    Vector3f textureColor = isec.object->texture(phit, isec.idx);

    //ambient
    Vector3f phitColor = material->kDiffuse * textureColor * scene.kAmbient;

    // if inside surface, inverting normal direction
    float cosi = normal.dot(ray.direction);
    if ( cosi > 0.0f) // inside surface
        normal = -normal;

    // diffuse and specular
    for(auto& light: scene.lights)
    {
        Vector3f toLight = light->direction(phit);

        float incidence = normal ^ toLight; //NdotL
        if( incidence > 0.0f )
        {
            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                Vector3f lightIntensity = light->intensity(phit);

                //diffuse
                Vector3f diffuse = material->kDiffuse * textureColor * incidence;

                //specular
                Vector3f toCamera = -ray.direction;
                Vector3f reflected = reflect(-toLight, normal);
                Vector3f specular = material->kSpecularHighlight * pow(std::max(0.0f, toCamera ^ reflected), material->shininess);
                phitColor +=  lightIntensity * (diffuse + specular);
            }
        }
    }
    phitColor *= 1.0f - material->reflectivity;

    Ray R;
    R.origin = phit + bias * normal;
    R.direction = reflect(ray.direction, normal).normalize();
    return phitColor + rayTrace(R, scene, depth + 1) * material->kSpecular * material->reflectivity;
}

inline
Vector3f Render::mirrorMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Vector3f phit = ray.origin + isec.tnear * ray.direction;
    Vector3f normal = isec.object->normal(phit, isec.idx);
    const Material *material = &isec.object->material;

    Ray R;
    R.origin = phit + bias * normal;
    R.direction = reflect(ray.direction, normal).normalize();
    return rayTrace(R, scene, depth + 1) * material->kSpecular;
}

inline
Vector3f Render::transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec)
{
    Vector3f phit = ray.origin + isec.tnear * ray.direction;
    Vector3f normal = isec.object->normal(phit, isec.idx);
    const Material *material = &isec.object->material;

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
                Vector3f specular = material->kSpecularHighlight * pow(std::max(0.0f, toCamera ^ reflected), material->shininess);
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
        n2 = material->refractiveIndex;
    }
    else              // inside surface
    {
        normal = -normal;
        n1 = material->refractiveIndex;
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

void Render::render(const Scene &scene, uint8_t nrays, uint8_t nthreads)
{       
    if (nthreads == 0 || nthreads >= options.getHeight())
        throw std::invalid_argument("# of threads must be > 0 and < image.height ");

    if (nrays == 0)
        throw std::invalid_argument("# of rays must be greater than 0");

    std::vector<std::thread> threads;

    size_t nrows = options.getHeight() / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( &Render::renderSingleThread, this, &scene, i*nrows, (i+1) * nrows, nrays ) );
    }
    threads.push_back( std::thread( &Render::renderSingleThread, this, &scene, i*nrows, options.getHeight(), nrays ) );

    for (auto& thread : threads) thread.join();
}
