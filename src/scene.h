#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "light.h"
#include "consts.h"
#include "cameraoptions.h"
#include "shape.h"

typedef std::vector<std::shared_ptr<Shape>> ObjectVector;

class Scene
{
public:
    Scene();
    Scene(const std::string &fileName);

    void addLight(Light * light);

    void addObject(std::shared_ptr<Shape> obj);

    void load(const std::string &fileName);

    std::string     fileName;
    std::string     name;
    float           ambientIndex;
    float           ka;
    size_t          spp;
    size_t          grid;
    size_t          maxDepth;
    Vector3         bgColor;
    RayTracerType   raytracer;

    CameraOptions   cameraOptions;
    ObjectVector    objects;
    LightVector     lights;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};

#endif // SCENE_H
