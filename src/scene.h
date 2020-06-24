#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "light.h"
#include "cameraoptions.h"
#include "shape.h"
#include "renderoptions.h"
#include "object.h"

class Scene
{
    typedef std::vector<std::shared_ptr<Object>> ObjectVector2;

    typedef std::vector<std::shared_ptr<Shape>> ObjectVector;
    typedef std::vector<std::shared_ptr<Light>> LightVector;

public:
    Scene();
    Scene(const std::string &fileName);
    ~Scene();

    void addLight(std::shared_ptr<Light> light);

    void addObject(std::shared_ptr<Shape> obj);
    void addObject(std::shared_ptr<Object> obj);


    void load(const std::string &fileName);

    std::string     fileName;
    std::string     name;

    RenderOptions   renderOptions;
    CameraOptions   cameraOptions;
    ObjectVector    objects2;
    LightVector     lights;

    ObjectVector2   objects;

    friend std::ostream &operator <<(std::ostream &os, const Scene &scene);
};

#endif // SCENE_H
