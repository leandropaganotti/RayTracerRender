#pragma once

#include <memory>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "scene.h"
#include "material.h"
#include "texture.h"

#include "sphere.h"
#include "plane.h"
#include "box.h"
#include "cylinder.h"
#include "mesh.h"
#include "object.h"

class XMLParser
{
public:
    XMLParser() = default;

    bool parseFile(const char *filename, Scene & scene);

    void parseScene(xmlNode * xmlSceneNode, Scene & scene);

    void parseCameraOptions(xmlNode * xmlCameraOptionsNode, CameraOptions & options);
    void parseRenderOptions(xmlNode * xmlRenderOptionsNode, RenderOptions & options);

    std::shared_ptr<Light> parsePointLight(xmlNode * xmlPointLightNode);
    std::shared_ptr<Light> parseDistantLight(xmlNode * xmlDistantLightNode);
    std::shared_ptr<Light> parseSphericalLight(xmlNode * xmlsSphericalLightNode);

    std::shared_ptr<Object>  parsePlane(xmlNode * xmlPlaneNode);
    std::shared_ptr<Object>  parseSphere(xmlNode * xmlSphereNode);
    std::shared_ptr<Object>  parseBox(xmlNode * xmlBoxNode);
    std::shared_ptr<Object>  parseCylinder(xmlNode * xmlCylinderNode);
    std::shared_ptr<Object>  parseMesh(xmlNode * xmlMeshNode);


    Matrix4 parseTransformation(xmlNode * xmlTrnasformationNode);
    std::shared_ptr<Material> parseMaterial(xmlNode * xmlMaterialNode);
    std::shared_ptr<Texture>  parseTexture(xmlNode * xmlTextureNode);

private:
    bool     equals(const xmlChar *lhs, const char *rhs);

    Vector3  toVector3(const xmlChar *str);

    float    toFloat(const xmlChar *str);

    int      toInt(const xmlChar *str);

    bool     toBool(const xmlChar *str);

    void LogError(const xmlNode *node, const xmlAttr* attr, const char *msg);
};

