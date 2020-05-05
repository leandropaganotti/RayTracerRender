#pragma once

#include <memory>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "scene.h"
#include "math/shapes/shapefactory.h"

class XMLParser
{
public:
    XMLParser() = default;

    void parseFile(const char *filename, Scene & scene);

    void parseScene(xmlNode * xmlSceneNode, Scene & scene);

    void parseCameraOptions(xmlNode * xmlCameraOptionsNode, CameraOptions & options);

    void parsePointLight(xmlNode * xmlPointLightNode, PointLight & light);

    void parseDistantLight(xmlNode * xmlDistantLightNode, DistantLight & light);

    std::shared_ptr<ShapeWithMaterial>  parseShape(xmlNode * xmlObjectNode);
    std::shared_ptr<ShapeWithMaterial>  parsePlane(xmlNode * xmlPlaneNode);
    std::shared_ptr<ShapeWithMaterial>  parseSphere(xmlNode * xmlSphereNode);
    std::shared_ptr<ShapeWithMaterial>  parseBox(xmlNode * xmlBoxNode);
    std::shared_ptr<ShapeWithMaterial>  parseCylinder(xmlNode * xmlCylinderNode);
    std::shared_ptr<ShapeWithMaterial>  parseEllipsoid(xmlNode * xmlEllipsoidNode);
    std::shared_ptr<ShapeWithMaterial>  parseMesh(xmlNode * xmlMeshNode);


    Matrix4 parseTransformation(xmlNode * xmlTrnasformationNode);
    std::shared_ptr<Material> parseMaterial(xmlNode * xmlMaterialNode);
    std::shared_ptr<Texture> parseTexture(xmlNode * xmlTextureNode);

private:
    bool     equals(const xmlChar *lhs, const char *rhs);

    Vector3 toVector(const xmlChar *str);

    float    toFloat(const xmlChar *str);

    int      toInt(const xmlChar *str);
};

