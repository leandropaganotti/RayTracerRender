#include "scene.h"
#include <stdio.h>
#include <string.h>
#include "xmlparser.h"

Scene::Scene()
{
    name = "unamed";
}

Scene::Scene(const std::string &fileName)
{
    name = "unamed";
    this->fileName = fileName;
    load(fileName);
}

Scene::~Scene(){}

void Scene::addLight(Light *light)
{
    lights.emplace_back(light);
}

void Scene::addObject(std::shared_ptr<Shape> obj)
{
    if(obj)
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
    "\t" << scene.renderOptions << std::endl;

    return os;
}



