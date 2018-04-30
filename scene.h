#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "light.h"

class Scene
{
public:
    Scene();
    ~Scene();

    ObjectVector objects;
    Vector3f backgroundColor;
    LightVector lights;

};

#endif // SCENE_H
