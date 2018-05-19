#include "scene.h"
#include <stdio.h>
#include <string.h>
#include "xmlparser.h"

Scene::Scene(): bgColor(0), ambientIndex(1.0f), kAmbient(0.1) {}

void Scene::addObject(Object *obj)
{
    objects.emplace_back(obj);
}

void Scene::addLight(Light *light)
{
    lights.emplace_back(light);
}

void Scene::load(const char *filename)
{
    XMLParser().parseFile(filename, this);
}

std::ostream &operator <<(std::ostream &os, const Scene &scene)
{
    os << "Scene: " << scene.cameraOptions << std::endl <<
          "bgColor: " << scene.bgColor << ", ambientIndex: " << scene.ambientIndex;
    return os;
}



