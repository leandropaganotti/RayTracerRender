#include "scene.h"
#include <stdio.h>
#include <string.h>
#include <xmlparser.h>

Scene::Scene(): name("unamed"), ambientIndex(1.0f), ka(0.1), spp(1), grid(1), maxDepth(3), bgColor(0), shade(Shader::PHONG){}

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
          "ambientIndex: " << scene.ambientIndex << ", ambientCoefficient: " << scene.ka;
    return os;
}



