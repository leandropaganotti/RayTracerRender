#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string.h>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "scene.h"
#include "sphere.h"
#include "plane.h"
#include "box.h"
#include "model.h"

using namespace std;

class XMLParser
{
public:
    XMLParser() = default;

    void parseFile(const char *filename, Scene & scene);

    void parseScene(xmlNode * xmlSceneNode, Scene & scene);

    void parseCameraOptions(xmlNode * xmlCameraOptionsNode, CameraOptions & options);

    void parseSphere(xmlNode * xmlSphereNode, Sphere & sphere);

    void parseMaterial(xmlNode * xmlMaterialNode, Material & material);

    void parsePointLight(xmlNode * xmlPointLightNode, PointLight & light);

    void parsePlane(xmlNode * xmlPlaneNode, Plane & plane);

    void parseTexture(xmlNode * xmlTextureNode, std::unique_ptr<Texture> &tex);

    void parseBox(xmlNode * xmlBoxNode, Box & box);

    void parseModel(xmlNode * xmlModelNode, ModelMatrix & model);

private:
    bool     equals(const xmlChar *lhs, const char *rhs);

    Vector3f toVector(const xmlChar *str);

    float    toFloat(const xmlChar *str);

    int      toInt(const xmlChar *str);
};

#endif // XMLPARSER_H
