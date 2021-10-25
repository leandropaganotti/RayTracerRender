#include "scene.h"
#include <stdio.h>
#include <string.h>
#include "xmlparser.h"

Scene::Scene(const std::string &fileName)
{
    objects = std::make_shared<BVH<Object>>();
    name = "unamed";
    this->fileName = fileName;
    readFromXMLFile(fileName);
}

void Scene::addLight(std::shared_ptr<Light> light)
{
    if(light)
        lights.push_back(light);
}

bool Scene::readFromXMLFile(const std::string &fileName)
{
    this->fileName = fileName;
    objects->clear();
    if(XMLParser().parseFile(fileName.c_str(), *this)){
        objects->build();
        return true;
    } else {
        return false;
    }
}

void Scene::addObject(std::shared_ptr<Object> &&o)
{
    if(o)
        objects->add(o);
}

bool Scene::isEmpty()
{
    return objects->size() == 0;
}

std::ostream &operator <<(std::ostream &os, const Scene &scene)
{
    os << "Scene: " << scene.name << " : " << scene.fileName << std::endl
    << "\t" << scene.cameraOptions << std::endl <<
    "\t" << scene.renderOptions << std::endl;

    return os;
}



