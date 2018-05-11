#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "light.h"
#include "camera.h"

class Scene
{
public:
	Scene() = default;

    ObjectVector objects;
    LightVector lights;
    Vector3f bgColor;
    CameraOptions cameraOptions;

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
