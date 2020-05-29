#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "light.h"
#include "consts.h"
#include "cameraoptions.h"
#include "shape.h"
#include "renderoptions.h"

class Scene
{
    typedef std::vector<std::shared_ptr<Shape>> ObjectVector;
    typedef std::vector<std::unique_ptr<Light>> LightVector;

public:
    Scene();
    Scene(const std::string &fileName);
    ~Scene();

    void addLight(std::unique_ptr<Light> &&light);

    void addObject(std::shared_ptr<Shape> obj);

    void load(const std::string &fileName);

    std::string     fileName;
    std::string     name;

    RenderOptions   renderOptions;
    CameraOptions   cameraOptions;
    ObjectVector    objects;
    LightVector     lights;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};

#endif // SCENE_H
