#include "scene.h"
#include <stdio.h>
#include <string.h>
#include "xmlparser.h"

Scene::Scene(): name("unamed"), ambientIndex(1.0f), ka(0.1), spp(1), grid(1), maxDepth(3), bgColor(0), raytracer(RayTracerType::RayTracerPhong){}

Scene::Scene(const std::string &fileName): name("unamed"), ambientIndex(1.0f), ka(0.1), spp(1), grid(1), maxDepth(3), bgColor(0), raytracer(RayTracerType::RayTracerPhong)
{
    load(fileName);
}

void Scene::addLight(Light *light)
{
    lights.emplace_back(light);
}

void Scene::addObject(std::shared_ptr<Shape> obj)
{
    objects.push_back(obj);
}

void Scene::load(const std::string &fileName)
{
    this->fileName = fileName;
    XMLParser().parseFile(fileName.c_str(), *this);
}

std::ostream &operator <<(std::ostream &os, const Scene &scene)
{
    os << "Scene: " << scene.name << " : " << scene.fileName << std::endl
    << "\t" << scene.cameraOptions << std::endl <<
    "\tambientIndex: " << scene.ambientIndex << ", ka: " << scene.ka << ", spp: " <<
    scene.spp << ", grid: " << scene.grid << ", bgColor: " << scene.bgColor << ", raytracer: " << (int)scene.raytracer << std::endl;

    return os;
}



