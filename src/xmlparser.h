#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "scene.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"

class XMLParser
{
    bool     equals(const xmlChar *lhs, const char *rhs);
    void     error(const xmlNode *node);
    Vector3f toVector(const xmlNode *node);
    float    toFloat(const xmlNode *node);
    int      toInt(const xmlNode *node);
public:
    XMLParser() = default;

    void parseFile(const char *filename, Scene & scene);

    void parseScene(xmlNode * xmlSceneNode, Scene & scene);

    void parseCameraOptions(xmlNode * xmlCameraOptionsNode, CameraOptions & options);

    void parseSphere(xmlNode * xmlSphereNode, Sphere & sphere);

    void parseMaterial(xmlNode * xmlMaterialNode, Material & material);

    void parsePointLight(xmlNode * xmlPointLightNode, PointLight & light);

    void parsePlane(xmlNode * xmlPlaneNode, Plane & plane);

};

#endif // XMLPARSER_H
