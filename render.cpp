#include "render.h"
#include <thread>

void Render::render(const Scene &scene, size_t width, size_t height, uint8_t nrays, uint8_t nthreads)
{
    if (nthreads ==0 || nthreads > 4)
        throw std::invalid_argument("# of threads must be 1, 2, 3 or 4");

    if (nrays < 1)
        throw std::invalid_argument("# of rays must be > 0");

    camera.setResolution(width, height);

    image.resize(width, height);

    std::vector<std::thread> threads;

    size_t nrows = image.height() / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( &Render::render_1x1x1, this, &scene, i*nrows, (i+1) * nrows, nrays ) );
    }
    threads.push_back( std::thread( &Render::render_1x1x1, this, &scene, i*nrows, image.height(), nrays ) );

    for (auto& thread : threads) thread.join();
}

void Render::render_1x1x1(const Scene *scene, size_t start, size_t end, size_t nrays)
{
    if (scene == nullptr) return;
    Ray ray(camera.getPosition(), 0.0f);

    // nrays defines a greid nrays x nrays
    float dx = 1.0f / ( 1.0f + nrays);
    float dy = 1.0f / ( 1.0f + nrays);

    for (size_t i = start; i < end; ++i)
    {
        for (size_t j = 0; j < image.width(); ++j)
        {
            image.at(i, j) = 0;
            for (size_t y=1; y <= nrays; ++y)
            {
                for (size_t x=1; x <= nrays; ++x)
                {
                    ray.direction = camera.getRayDirection(i+dy*y, j+dx*x);
                    image.at(i, j) += trace(ray, *scene, 0);
                }
            }
            image.at(i, j) /= nrays*nrays;
        }
    }
}

Vector3f Render::trace(const Ray &ray, const Scene &scene, const uint8_t depth)
{
    if(depth == MAX_DEPTH) return scene.bgColor;

    IntersectionData isec;

    castRay(ray, scene.objects, isec);

    if (isec.object == nullptr) return scene.bgColor;

    //Phong shading down here

    Vector3f phit = ray.origin + isec.tnear * ray.direction;
    Vector3f normal = isec.object->normal(phit, isec.idx);

    //ambient
    Vector3f phitColor = isec.object->k_ambient;

    // diffuse and specular
    for(auto& light: scene.lights)
    {
        Vector3f toLight = light->direction(phit);

        float incidence = normal ^ toLight;
        if( incidence > 0.0f )
        {
        	if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
			{
				Vector3f lightIntensity = light->intensity(phit);

				//diffuse
				phitColor += lightIntensity * isec.object->k_diffuse * incidence;

				//specular
				Vector3f toCamera = (ray.origin - phit).normalize();
				Vector3f reflected = reflect(-toLight, normal);
				phitColor += lightIntensity * isec.object->k_specular * pow(std::max(0.0f, toCamera ^ reflected), isec.object->shininess);
			}
        }
    }

    //reflection
    if( isec.object->reflectivity )
    {
        Ray reflected( phit + bias * normal, reflect(ray.direction, normal) );
        phitColor += trace(reflected, scene, depth + 1) * isec.object->reflectivity ;
    }

    return phitColor;
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
            if ( tnear < tMax ) return true;
        }
    }
    return false;
}
