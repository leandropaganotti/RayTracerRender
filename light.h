#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include <vector>

class Light
{
public:
    Light();
    ~Light();

    union {
        Vector3f origin;
        Vector3f direction;
    };

    Vector3f color;
    float strength;
    float k_ambient;
    float attenuation;
};

typedef std::vector<Light> LightVector;

#endif // LIGHT_H
