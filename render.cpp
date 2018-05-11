#include "render.h"
#include <thread>

void Render::render(const Scene &scene, size_t width, size_t height, uint8_t nrays, uint8_t nthreads)
{
    if (nthreads ==0 || nthreads > 4)
        throw std::invalid_argument("# of threads must be 1, 2, 3 or 4");

    if (nrays < 1)
        throw std::invalid_argument("# of rays must be > 0");

    camera.setResolution(width * nrays, height * nrays);

    image.resize(width * nrays, height * nrays);

    std::vector<std::thread> threads;

    size_t nrows = image.height() / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( &Render::render_1x1x1, this, &scene, i*nrows, (i+1) * nrows ) );
    }
    threads.push_back( std::thread( &Render::render_1x1x1, this, &scene, i*nrows, image.height() ) );

    for (auto& thread : threads) thread.join();

    if( nrays > 1)
    {
        Image frame( width, height );

        for (size_t i = 0; i < frame.height(); ++i)
        {
            for (size_t j = 0; j < frame.width(); ++j)
            {
                frame.at(i, j) = 0;
                for (size_t k = i*nrays; k < i*nrays + nrays; ++k)
                {
                    for (size_t l = j*nrays; l < j*nrays + nrays; ++l)
                    {
                        frame.at(i, j) += image.at(k, l);
                    }
                }
                frame.at(i, j) /= nrays*nrays;
            }
        }
        image.move(frame);
    }
}

void Render::render_1x1x1(const Scene *scene, size_t start, size_t end)
{
    if (scene == nullptr) return;
    Ray ray(camera.getPosition(), 0.0f);
    for (size_t i = start; i < end; ++i)
    {
        for (size_t j = 0; j < image.width(); ++j)
        {
            ray.direction = camera.getRayDirection(i, j);
            image.at(i, j) = trace(ray, *scene, 0);
        }
    }
}

Vector3f Render::trace(const Ray &ray, const Scene &scene, const uint8_t depth)
{
    if(depth == MAX_DEPTH) return scene.bgColor;

    IntersectionData isec;

    castRay(ray, scene.objects, isec);

    if (isec.object == nullptr) return scene.bgColor;

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
