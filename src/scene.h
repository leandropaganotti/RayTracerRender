#pragma once

#include <string>
#include <vector>
#include "light.h"
#include "cameraoptions.h"
#include "renderoptions.h"
#include "bvh.h"

class Scene
{
public:
    Scene(const std::string &fileName="");
    ~Scene() = default;

    void addLight(std::shared_ptr<Light> light);
    void addObject(std::shared_ptr<Object> &&o);
    bool isEmpty();
    bool readFromXMLFile(const std::string &fileName);

    std::string     fileName;
    std::string     name;
    RenderOptions   renderOptions;
    CameraOptions   cameraOptions;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Aggregate<Object>>  objects;

    bool intersection(const Ray &ray, IntersectionData &isec) const
    {
        return objects->intersection(ray, isec);
    }
    bool intersection(const Ray &ray) const
    {
        return objects->intersection(ray);
    }

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};
