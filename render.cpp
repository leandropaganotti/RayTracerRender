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

Image &Render::getImage()
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
    if(depth == MAX_DEPTH) return scene.bgColor;

    IntersectionData isec;

    castRay(ray, scene.objects, isec);

    if (isec.object == nullptr) return scene.bgColor;

    //Phong shading down here

    Vector3f phit = ray.origin + isec.tnear * ray.direction;
    Vector3f normal = isec.object->normal(phit, isec.idx);

    //ambient
    Vector3f phitColor = isec.object->k_diffuse * scene.kAmbient;

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
				phitColor += lightIntensity * isec.object->k_diffuse * incidence;

				//specular
				Vector3f toCamera = (ray.origin - phit).normalize();
                Vector3f reflected = reflect(-toLight, normal);
				phitColor += lightIntensity * isec.object->k_specular * pow(std::max(0.0f, toCamera ^ reflected), isec.object->shininess);
			}
        }
    }

    //reflection
    if(isec.object->type == Object::Type::REFLECTIVE && isec.object->reflectivity)
    {
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        phitColor += rayTrace(R, scene, depth + 1) * isec.object->reflectivity ;
    }
    else if(isec.object->type == Object::Type::REFRACTIVE)
    {

        float n, n1, n2, kr, kt;
        float cosi = normal.dot(ray.direction);
        if ( cosi < 0.0f) // outside
        {
            cosi = -cosi;
            n1 = scene.ambientIndex;
            n2 = isec.object->refractiveIndex;
        }
        else              // inside
        {
            normal = -normal;
            n1 = isec.object->refractiveIndex;
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
        Ray R;
        R.origin = phit + bias * normal;
        R.direction = reflect(ray.direction, normal).normalize();
        phitColor += rayTrace(R, scene, depth + 1) * kr ;

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
            if ( tnear < tMax && object->type != Object::Type::REFRACTIVE ) return true;
            //if ( tnear < tMax ) return true;
        }
    }
    return false;
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
