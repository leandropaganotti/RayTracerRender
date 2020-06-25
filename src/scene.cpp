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

    buildBVH();
}

std::ostream &operator <<(std::ostream &os, const Scene &scene)
{
    os << "Scene: " << scene.name << " : " << scene.fileName << std::endl
    << "\t" << scene.cameraOptions << std::endl <<
    "\t" << scene.renderOptions << std::endl;

    return os;
}



