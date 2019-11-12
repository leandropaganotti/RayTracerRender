#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "object.h"
#include "light.h"
#include "consts.h"
#include "cameraoptions.h"

class Scene
{
public:
    Scene();
    Scene(const std::string &fileName);

    void addObject(Object * obj);
    void addLight(Light * light);

    void load(const std::string &fileName);

    std::string     fileName;
    std::string     name;
    float           ambientIndex;
    float           ka;
    size_t          spp;
    size_t          grid;
    size_t          maxDepth;
    Vector3         bgColor;
    Shader          shader;

    CameraOptions   cameraOptions;
    ObjectVector    objects;
    LightVector     lights;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};

#endif // SCENE_H
