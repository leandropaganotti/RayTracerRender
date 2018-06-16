#include "scene.h"
#include <stdio.h>
#include <string.h>
#include "xmlparser.h"

Scene::Scene(): name("unamed"), ambientIndex(1.0f), kAmbient(0.1), nprays(1), nsrays(1), nshrays(1), maxDepth(3)  {}

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
    XMLParser().parseFile(filename, *this);
}

std::ostream &operator <<(std::ostream &os, const Scene &scene)
{
    os << "Scene: " << scene.cameraOptions << std::endl <<
          "ambientIndex: " << scene.ambientIndex << ", ambientCoefficient: " << scene.kAmbient;
    return os;
}



