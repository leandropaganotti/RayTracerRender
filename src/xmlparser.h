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
#include "transformation.h"
#include "objmodel.h"
#include "object.h"
#include <memory>

using namespace std;

class XMLParser
{
public:
    XMLParser() = default;

    void parseFile(const char *filename, Scene & scene);

    void parseScene(xmlNode * xmlSceneNode, Scene & scene);

    void parseCameraOptions(xmlNode * xmlCameraOptionsNode, CameraOptions & options);

    void parsePointLight(xmlNode * xmlPointLightNode, PointLight & light);

    void parseDistantLight(xmlNode * xmlDistantLightNode, DistantLight & light);

    std::unique_ptr<Object> parseObject(xmlNode * xmlObjectNode);
    std::shared_ptr<Shape>  parsePlane(xmlNode * xmlPlaneNode);
    std::shared_ptr<Shape>  parseSphere(xmlNode * xmlSphereNode);
    std::shared_ptr<Shape>  parseBox(xmlNode * xmlBoxNode);
    std::shared_ptr<Shape>  parseCylinder(xmlNode * xmlCylinderNode);
    std::shared_ptr<Shape>  parseEllipsoid(xmlNode * xmlEllipsoidNode);
    std::shared_ptr<Shape>  parseMesh(xmlNode * xmlMeshNode);


    Matrix4 parseTransformation(xmlNode * xmlTrnasformationNode);
    std::shared_ptr<Material> parseMaterial(xmlNode * xmlMaterialNode);
    std::shared_ptr<Texture> parseTexture(xmlNode * xmlTextureNode);

private:
    bool     equals(const xmlChar *lhs, const char *rhs);

    Vector3 toVector(const xmlChar *str);

    float    toFloat(const xmlChar *str);

    int      toInt(const xmlChar *str);
};

#endif // XMLPARSER_H
