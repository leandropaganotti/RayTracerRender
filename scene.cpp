#include "scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
    std::cout << "Scene: destrying objects" << std::endl;
    for (std::vector<Object*>::iterator it=objects.begin(); it != objects.end(); ++it)
    {
        delete *it;
    }
}

