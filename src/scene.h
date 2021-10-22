#pragma once

#include <string>
#include <vector>
#include "light.h"
#include "cameraoptions.h"
#include "renderoptions.h"

#include "aggregate.h"

class Scene: public Shape
{
public:
    Scene(const std::shared_ptr<Aggregate> &agg);
    Scene(const std::string &fileName, const std::shared_ptr<Aggregate> &agg);
    ~Scene();

    void addLight(std::shared_ptr<Light> light);

    void readFromXMLFile(const std::string &fileName);

    std::string     fileName;
    std::string     name;

    RenderOptions   renderOptions;
    CameraOptions   cameraOptions;

    std::vector<std::shared_ptr<Light>>     lights;
    std::vector<std::shared_ptr<Object>>    objects;


    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);

    std::shared_ptr<Aggregate> aggregate;

    void addObject(std::shared_ptr<Object> &&o);

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        return aggregate->intersection(ray, isec);
    }
    bool intersection(const Ray &ray) const override
    {
        return aggregate->intersection(ray);
    }
    AABB getAABB() const override
    {
        return aggregate->getAABB();
    }
};
