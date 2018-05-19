#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "object.h"
#include "light.h"
#include "camera.h"

class Scene
{
public:
    Scene();

    void addObject(Object * obj);
    void addLight(Light * light);

    void load(const char* filename);

    Vector3f        bgColor;
    float           ambientIndex;
    float           kAmbient;

    CameraOptions   cameraOptions;
    ObjectVector    objects;
    LightVector     lights;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};

#endif // SCENE_H
