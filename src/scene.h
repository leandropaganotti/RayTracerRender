#pragma once

#include <string>
#include <vector>
#include "light.h"
#include "cameraoptions.h"
#include "renderoptions.h"
#include "bvh.h"

class Scene: public Shape
{
public:
    Scene(const std::shared_ptr<Aggregate<Object>> &agg);
    Scene(const std::string &fileName="", const std::shared_ptr<Aggregate<Object>> &agg=std::make_shared<BVH<Object>>());
    ~Scene() = default;

    void addLight(std::shared_ptr<Light> light);

    void readFromXMLFile(const std::string &fileName);

    std::string     fileName;
    std::string     name;

    RenderOptions   renderOptions;
    CameraOptions   cameraOptions;

    std::vector<std::shared_ptr<Light>>     lights;


    std::shared_ptr<Aggregate<Object>> objects;

    void addObject(std::shared_ptr<Object> &&o);

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        return objects->intersection(ray, isec);
    }
    bool intersection(const Ray &ray) const override
    {
        return objects->intersection(ray);
    }
    AABB getAABB() const override
    {
        return objects->getAABB();
    }

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};
