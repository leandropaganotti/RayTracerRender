#include "scene.h"
#include <stdio.h>
#include <string.h>
#include "xmlparser.h"

Scene::Scene(const std::shared_ptr<Aggregate> &agg): aggregate(agg)
{
    name = "unamed";
}

Scene::Scene(const std::string &fileName, const std::shared_ptr<Aggregate> &agg)
{
    aggregate = agg;
    name = "unamed";
    this->fileName = fileName;
    readFromXMLFile(fileName);
}

Scene::~Scene(){}

void Scene::addLight(std::shared_ptr<Light> light)
{
    if(light)
        lights.push_back(light);
}

void Scene::readFromXMLFile(const std::string &fileName)
{
    this->fileName = fileName;
    XMLParser().parseFile(fileName.c_str(), *this);
    aggregate->create(objects);
}

void Scene::addObject(std::shared_ptr<Object> &&o)
{
    if(o)
    objects.push_back(o);
}

std::ostream &operator <<(std::ostream &os, const Scene &scene)
{
    os << "Scene: " << scene.name << " : " << scene.fileName << std::endl
    << "\t" << scene.cameraOptions << std::endl <<
    "\t" << scene.renderOptions << std::endl;

    return os;
}



