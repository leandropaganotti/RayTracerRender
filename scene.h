#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "light.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    ObjectVector objects;
    LightVector lights;

    Vector3f bgColor;

    void addObject(Object * obj)
    {
        objects.emplace_back(obj);
    }

    void addLight(Light * light)
    {
        lights.emplace_back(light);
    }

};

#endif // SCENE_H
