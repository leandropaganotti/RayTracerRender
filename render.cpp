#include "render.h"
#include <thread>

void Render::render(const Scene &scene, size_t width, size_t height, uint8_t nrays, uint8_t nthreads)
{
    if (nthreads ==0 || nthreads > 4)
        throw std::invalid_argument("# of threads must be 1, 2, 3 or 4");

    if (nrays < 1)
        throw std::invalid_argument("# of rays must be > 0");

    _frame.resize( width * nrays, height * nrays);

    _camera.resolution(_frame.width(), _frame.height());

    std::vector<std::thread> threads;

    size_t nrows = _frame.height() / nthreads, i;

    for( i=0 ; i < nthreads - 1u; ++i)
    {
        threads.push_back( std::thread( &Render::render_1x1x1, this, &scene, i*nrows, (i+1) * nrows ) );
    }
    threads.push_back( std::thread( &Render::render_1x1x1, this, &scene, i*nrows, _frame.height() ) );

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
                        frame.at(i, j) += _frame.at(k, l);
                    }
                }
                frame.at(i, j) /= nrays*nrays;
            }
        }
        _frame.move(frame);
    }
}

void Render::render_1x1x1(const Scene *scene, size_t start, size_t end)
{
    if (scene == nullptr) return;
    Ray ray(_camera.position(), 0.0f);
    for (size_t i = start; i < end; ++i)
    {
        for (size_t j = 0; j < _frame.width(); ++j)
        {
            ray.direction = _camera.rayDirection(i, j);
            _frame.at(i, j) = trace(ray, *scene);
        }
    }
}

Vector3f Render::trace(const Ray &ray, const Scene &scene)
{
    IntersectionData isec;

    castRay(ray, scene.objects, isec);

    if (isec.object)
    {
        //ambient color
        Vector3f phitColor = isec.object->ambient();
        Vector3f phit = ray.origin + isec.tnear * ray.direction;
        Vector3f normal = isec.object->normal(phit, isec.idx);

        for(auto& light: scene.lights)
        {
            Vector3f toLight = light->direction(phit);

            if (!castShadowRay(Ray(phit + bias * normal, toLight), scene.objects, light->distance(phit)))
            {
                //diffuse NdotL
                float NdotL = std::max(0.0f, normal ^ toLight);
                phitColor += light->intensity(phit) * isec.object->diffColor() * NdotL;

                //specular
                if( NdotL > 0.0f )
                {
                    //specular color
                    Vector3f toCamera = (ray.origin - phit).normalize();
                    Vector3f reflection = (2.0f * ((normal ^ toLight) * normal) - toLight);
                    phitColor += light->intensity(phit) * isec.object->specColor() * pow(std::max(0.0f, toCamera ^ reflection), isec.object->shininess);
                }
            }
        }

        //gamma
        const float gamma=1.0/2.2;
        phitColor[0] = pow(phitColor[0], gamma);
        phitColor[1] = pow(phitColor[1], gamma);
        phitColor[2] = pow(phitColor[2], gamma);

        return phitColor;
    }
    return scene.bgColor;
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
