#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "object.h"
#include "light.h"
#include "camera.h"
#include "consts.h"

class Scene
{
public:
    Scene();

    void addObject(Object * obj);
    void addLight(Light * light);

    void load(const char* filename);

    std::string     name;
    float           ambientIndex;
    float           ka;
    size_t          spp;
    size_t          grid;
    size_t          maxDepth;
    Vector3        bgColor;    
    Shade           shade;

    CameraOptions   cameraOptions;
    ObjectVector    objects;
    LightVector     lights;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};

#endif // SCENE_H
