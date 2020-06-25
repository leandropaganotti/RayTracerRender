#pragma once

#include <string>
#include <vector>
#include "light.h"
#include "cameraoptions.h"
#include "renderoptions.h"

#include "object.h"

class Scene: public GroupedObject
{
public:
    Scene();
    Scene(const std::string &fileName);
    ~Scene();

    void addLight(std::shared_ptr<Light> light);

    void readFromXMLFile(const std::string &fileName);

    std::string     fileName;
    std::string     name;

    RenderOptions   renderOptions;
    CameraOptions   cameraOptions;

    std::vector<std::shared_ptr<Light>>     lights;
    std::vector<std::shared_ptr<Object>>   objects;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};
