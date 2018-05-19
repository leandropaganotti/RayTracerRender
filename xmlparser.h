#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "scene.h"
#include "sphere.h"

class XMLParser
{
    bool     equals(const xmlChar *lhs, const char *rhs);
    void     error(const xmlNode *node);
    Vector3f toVector(const xmlNode *node);
    float    toFloat(const xmlNode *node);
    int      toInt(const xmlNode *node);
public:
    XMLParser() = default;

    void parseFile(const char *filename, Scene * const scene);

    void parseScene(xmlNode * xmlSceneNode, Scene * const scene);

    void parseCameraOptions(xmlNode * xmlCameraOptionsNode, CameraOptions * const options);

    void parseSphere(xmlNode * xmlSphereNode, Sphere * const sphere);

    void parseMaterial(xmlNode * xmlMaterialNode);

    void parsePointLight(xmlNode * xmlPointLightNode, PointLight *light);

};

#endif // XMLPARSER_H
