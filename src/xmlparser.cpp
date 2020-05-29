#include "xmlparser.h"
#include <cstring>
#include "utils.h"
#include "objparser.h"
#include "transformation.h"

bool XMLParser::equals(const xmlChar *lhs, const char *rhs)
{
    if(lhs == nullptr || rhs == nullptr) return false;

    unsigned int sz = strlen( (const char*) lhs);

    if (strlen(rhs) != sz) return false;

    for (unsigned i = 0; i < sz; ++i)
        if (tolower(lhs[i]) != tolower(rhs[i]))
            return false;
    return true;
}

Vector3 XMLParser::toVector(const xmlChar *str)
{
    if (str == NULL) return Vector3(0.0f);

    std::string text((const char*)str);
    size_t first = text.find_first_of(',');

    // if Vector is only one float : Vector3(x)
    if (first == std::string::npos)
    {
        return Vector3(stof(text));
    }
    // if Vector is 3 floats: Vector3(x, y, z)
    else
    {
        Vector3 vec;
        vec.x = stof(text.substr(0, first));
        text.erase(0, first+1);
        size_t second = text.find_first_of(',');
        if (second == std::string::npos)
        {
            return Vector3(0.0f);
        }
        else
        {
            vec.y = stof(text.substr(0, second));
            text.erase(0, second+1);
            if (text.size() == 0)
            {

                return Vector3(0.0f);
            }
            else
                vec.z = stof(text);
        }
        return vec;
    }
}

float XMLParser::toFloat(const xmlChar *str)
{
    return str == NULL ? 0.0 : atof((const char*)str);
}

int XMLParser::toInt(const xmlChar *str)
{
    return str == NULL ? 0 : atoi((const char*)str);
}

void XMLParser::LogError(const xmlNode *node, const xmlAttr* attr, const std::string &msg)
{

    std::cerr << "\x1b[33;1m Error: <" << node->name << " " << attr->name << "=\'" << (const char*)attr->children->content << "\'>: " << msg << "\x1b[0m" << std::endl;

}

void XMLParser::parseFile(const char *filename, Scene & scene)
{
    xmlDoc *doc = NULL;
    xmlNode *xmlSceneNode = NULL;

    /*
    * this initialize the library and check potential ABI mismatches
    * between the version it was compiled for and the actual shared
    * library used.
    */
    LIBXML_TEST_VERSION

    doc = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);

    if (doc == NULL)
    {
        std::cerr << "\x1b[33;1m" << "error: could not parse file " << filename << "\x1b[0m" << std::endl;
    }
    else
    {
        xmlSceneNode = xmlDocGetRootElement(doc);

        // start parsing the xml tree here
        parseScene(xmlSceneNode, scene);

        xmlFreeDoc(doc);

        xmlCleanupParser();
    }
}

void XMLParser::parseScene(xmlNode *xmlSceneNode, Scene & scene)
{
    if(xmlSceneNode == NULL)
    {
        std::cerr << "\x1b[33;1m" << "error: could not parse Scene, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
        return;
    }

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlSceneNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlSceneNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    if(xmlSceneNode->properties && equals(xmlSceneNode->properties->name, "name"))
        scene.name.assign((const char*)xmlSceneNode->properties->children->content);

    xmlNode *node = NULL;
    for (node = xmlSceneNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "CameraOptions"))
                parseCameraOptions(node, scene.cameraOptions);
            else if (equals(node->name, "RenderOptions"))
                parseRenderOptions(node, scene.renderOptions);
            else if(equals(node->name, "PointLight"))
            {
                scene.addLight(parsePointLight(node));
            }
            else if(equals(node->name, "DistantLight"))
            {
                scene.addLight(parseDistantLight(node));
            }
            else if (equals(node->name, "sphere"))
            {
                scene.addObject(parseSphere(node));
            }
            else if (equals(node->name, "ellipsoid"))
            {
                scene.addObject(parseEllipsoid(node));
            }
            else if (equals(node->name, "box"))
            {
                scene.addObject(parseBox(node));
            }
            else if (equals(node->name, "cylinder"))
            {
                scene.addObject(parseCylinder(node));
            }
            else if (equals(node->name, "plane"))
            {
                scene.addObject(parsePlane(node));
            }
            else if (equals(node->name, "mesh"))
            {
                scene.addObject(parseMesh(node));
            }
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlSceneNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }
}

void XMLParser::parseCameraOptions(xmlNode *xmlCameraOptionsNode, CameraOptions & options)
{
    if(xmlCameraOptionsNode == NULL)
    {
        std::cerr << "\x1b[33;1m" << "error: could not parse CameraOptions, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
        return;
    }

    const xmlAttr *attr = NULL;
    for (attr = xmlCameraOptionsNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "position"))
            options.setFrom( toVector(attr->children->content) );
        else if (equals(attr->name, "lookingat"))
            options.setTo( toVector(attr->children->content) );
        else if (equals(attr->name, "fov"))
            options.setFov( deg2rad(toFloat(attr->children->content) ));
        else if (equals(attr->name, "width"))
            options.setWidth( toInt(attr->children->content) );
        else if (equals(attr->name, "height"))
            options.setHeight( toInt(attr->children->content) );
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlCameraOptionsNode->name << "\'" << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlCameraOptionsNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlCameraOptionsNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }
}

void XMLParser::parseRenderOptions(xmlNode *xmlRenderOptionsNode, RenderOptions &options)
{
    if(xmlRenderOptionsNode == NULL)
    {
        std::cerr << "\x1b[33;1m" << "error: could not parse RenderOptions, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
        return;
    }

    const xmlAttr *attr = NULL;
    for (attr = xmlRenderOptionsNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "spp"))
             options.spp = toInt(attr->children->content);
        else if (equals(attr->name, "grid"))
             options.gridSizeXY = toInt(attr->children->content);
        else if (equals(attr->name, "index"))
             options.ambientIndex = toFloat(attr->children->content);
        else if (equals(attr->name, "maxdepth"))
             options.maxDepth = toInt(attr->children->content);
        else if (equals(attr->name, "bgcolor"))
             options.bgColor = toVector(attr->children->content);
        else if (equals(attr->name, "illum"))
        {
            if (equals(attr->children->content, "phong"))
                options.illum = Illumination::Phong;
            else if (equals(attr->children->content, "global"))
                options.illum = Illumination::GlobalIlumination;
            else if (equals(attr->children->content, "global2"))
                options.illum = Illumination::GlobalIluminationWithDirectSampling;
            else
                std::cerr << "\x1b[33;1m" << "unrecognized attribute raytrace value \'" << attr->children->content << "\' " << "\x1b[0m" << std::endl;
        }
    }

    xmlNode *node = NULL;
    for (node = xmlRenderOptionsNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlRenderOptionsNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }
}

std::shared_ptr<Material> XMLParser::parseMaterial(xmlNode *xmlMaterialNode)
{
    if(xmlMaterialNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Material, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    xmlChar *attName = xmlGetProp(xmlMaterialNode,(const xmlChar*)"name");
    std::shared_ptr<Material> material = Material::Create(attName? (const char*)attName : "");
    xmlFree(attName);
    const xmlAttr *attr = NULL;
    std::string name = "";
    for (attr = xmlMaterialNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "kd"))
            material->Kd = toVector(attr->children->content);
        else if (equals(attr->name, "ka"))
            material->Ka = toVector(attr->children->content);
        else if (equals(attr->name, "E"))
            material->E = toVector(attr->children->content);
        else if (equals(attr->name, "ks"))
            material->Ks = toFloat(attr->children->content);
        else if (equals(attr->name, "m"))
            material->Ns = toFloat(attr->children->content);
        else if (equals(attr->name, "R0"))
            material->R0 = toFloat(attr->children->content);
        else if (equals(attr->name, "index"))
            material->Ni = toFloat(attr->children->content);
        else if (equals(attr->name, "type"))
        {
            if (equals(attr->children->content, "DIFFUSE"))
                material->type = MaterialType::DIFFUSE;
            else if (equals(attr->children->content, "SPECULAR"))
                material->type = MaterialType::SPECULAR;
            else if (equals(attr->children->content, "TRANSPARENT"))
                material->type = MaterialType::TRANSPARENT;
            else
                std::cerr << "\x1b[33;1m" << "unrecognized material type \'" << attr->children->content << "\'" << "\x1b[0m" << std::endl;
        }
        else if (equals(attr->name, "texture"))
        {
            material->texture = Resource::Get<Texture>((const char*)attr->children->content);
            if(!material->texture)
                LogError(xmlMaterialNode, attr, "Can't find Texture");
        }
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlMaterialNode->name << "\':" << name << "    \x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlMaterialNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "texture"))
                material->texture = parseTexture(node);
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlMaterialNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }
    return material;
}

std::unique_ptr<Light> XMLParser::parsePointLight(xmlNode *xmlPointLightNode)
{
    if(xmlPointLightNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse PointLight, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    xmlChar *attShadow = xmlGetProp(xmlPointLightNode,(const xmlChar*)"shadow");
    auto light = Light::Create(LightType::PointLight, !equals(attShadow, "false"));
    xmlFree(attShadow);

    const xmlAttr *attr = NULL;
    std::string name("");
    PointLight *lightRef = static_cast<PointLight*>(light.get());
    for (attr = xmlPointLightNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "shadow"))
           ;
        else if (equals(attr->name, "position"))
            lightRef->setPosition( toVector(attr->children->content) );
        else if (equals(attr->name, "color"))
            lightRef->setColor( toVector(attr->children->content) );
        else if (equals(attr->name, "strength"))
            lightRef->setStrength( toFloat(attr->children->content) );
        else if (equals(attr->name, "k"))
            lightRef->setK( toFloat(attr->children->content) );
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlPointLightNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlPointLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlPointLightNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }
    return light;
}

std::unique_ptr<Light> XMLParser::parseDistantLight(xmlNode *xmlDistantLightNode)
{
    if(xmlDistantLightNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse PointLight, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    xmlChar *attShadow = xmlGetProp(xmlDistantLightNode,(const xmlChar*)"shadow");
    auto light = Light::Create(LightType::DistantLight, !equals(attShadow, "false"));
    xmlFree(attShadow);

    const xmlAttr *attr = NULL;
    std::string name("");
    DistantLight *lightRef = static_cast<DistantLight*>(light.get());
    for (attr = xmlDistantLightNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "shadow"))
           ;
        else if (equals(attr->name, "direction"))
            lightRef->setDirection( toVector(attr->children->content) );
        else if (equals(attr->name, "color"))
            lightRef->setColor( toVector(attr->children->content) );
        else if (equals(attr->name, "strength"))
            lightRef->setStrength( toFloat(attr->children->content) );
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlDistantLightNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlDistantLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlDistantLightNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }
    return light;
}

std::shared_ptr<GPlane> XMLParser::parsePlane(xmlNode *xmlPlaneNode)
{
    if(xmlPlaneNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Plane, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    auto plane = std::shared_ptr<GPlane>( new GPlane);

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlPlaneNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "point"))
            plane->setO(toVector(attr->children->content));
        else if (equals(attr->name, "normal"))
            plane->setN(toVector(attr->children->content));
        else if (equals(attr->name, "material"))
            plane->setMaterial(Material::Get((const char*)attr->children->content));
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlPlaneNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlPlaneNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                plane->setMaterial(parseMaterial(node));
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlPlaneNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return plane;
}

std::shared_ptr<GSphere> XMLParser::parseSphere(xmlNode * xmlSphereNode)
{
    if(xmlSphereNode == NULL)
    {
        std::cerr << "\x1b[33;1m" << "error: could not parse Sphere, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
        return nullptr;
    }

    auto sphere = std::shared_ptr<GSphere>(new GSphere);

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlSphereNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "position"))
            sphere->setCenter( toVector(attr->children->content) );
        else if (equals(attr->name, "radius"))
            sphere->setRadius( toFloat(attr->children->content) );
        else if (equals(attr->name, "material"))
        {
            auto material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlSphereNode, attr, "Can't find material");
            sphere->setMaterial(material);
        }
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlSphereNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlSphereNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                sphere->setMaterial(parseMaterial(node));
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlSphereNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return sphere;
}

std::shared_ptr<Texture> XMLParser::parseTexture(xmlNode *xmlTextureNode)
{
    if(xmlTextureNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Texture, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    const xmlAttr *attr = NULL;
    std::string name("");
    std::string type("");
    std::string src("");
    float rows=1.0f, cols=1.0f, uedge=0.1f, vedge=0.1f, angle=0.0f;
    Vector3 color1=0, color2=1;
    for (attr = xmlTextureNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "type"))
            type = (const char*)attr->children->content;
        else if (equals(attr->name, "src"))
            src = (const char*)attr->children->content;
        else if (equals(attr->name, "rows"))
            rows = toFloat(attr->children->content);
        else if (equals(attr->name, "cols"))
            cols = toFloat(attr->children->content);
        else if (equals(attr->name, "uedge"))
            uedge = toFloat(attr->children->content);
        else if (equals(attr->name, "vedge"))
            vedge = toFloat(attr->children->content);
        else if (equals(attr->name, "color1"))
            color1 = toVector(attr->children->content);
        else if (equals(attr->name, "color2"))
            color2 = toVector(attr->children->content);
        else if (equals(attr->name, "angle"))
            angle = toFloat(attr->children->content);
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlTextureNode->name << "\'" << "\x1b[0m" << std::endl;
    }

    if (type == "Tiles")
    {
        auto tex = Tiles::Create(name, color1, color2, rows, cols, angle, uedge, vedge);
        return tex;
    }
    else if (type == "ChessBoard")
    {
        auto tex = ChessBoard::Create(name, color1, color2, rows, cols, angle);
        return tex;
    }
    else if (type == "Texture2d")
    {
        auto tex = Texture2d::Create(src, src);
        if(tex)
        {
            tex->setGridSizeU(cols);
            tex->setGridSizeV(rows);
        }
        return tex;
    }

    std::cerr << "\x1b[33;1m" << "Error parsing Texture, \'Type\' not found or invalid in " << xmlTextureNode->name << "\x1b[0m" << std::endl;
    return nullptr;
}

std::shared_ptr<GBox> XMLParser::parseBox(xmlNode *xmlBoxNode)
{
    if(xmlBoxNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Box, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    auto box = std::shared_ptr<GBox>(new GBox);

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlBoxNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "material"))
        {
            auto material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlBoxNode, attr, "Can't find material");
            box->setMaterial(material);
        }
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlBoxNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlBoxNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                box->setMaterial(parseMaterial(node));
            else if (equals(node->name, "transformation"))
                box->setTransformation(parseTransformation(node));
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlBoxNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return box;
}

std::shared_ptr<GCylinder> XMLParser::parseCylinder(xmlNode *xmlCylinderNode)
{
    if(xmlCylinderNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Box, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    auto cylinder = std::shared_ptr<GCylinder>(new GCylinder);

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlCylinderNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "material"))
        {
            auto material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlCylinderNode, attr, "Can't find material");
            cylinder->setMaterial(material);
        }
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlCylinderNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlCylinderNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                cylinder->setMaterial(parseMaterial(node));
            else if (equals(node->name, "transformation"))
                cylinder->setTransformation(parseTransformation(node));
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlCylinderNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return cylinder;
}

std::shared_ptr<GEllipsoid> XMLParser::parseEllipsoid(xmlNode *xmlEllipsoidNode)
{
    if(xmlEllipsoidNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Box, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    auto ellipsoid = std::shared_ptr<GEllipsoid>(new GEllipsoid);

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlEllipsoidNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "material"))
        {
            auto material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlEllipsoidNode, attr, "Can't find material");
            ellipsoid->setMaterial(material);
        }
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlEllipsoidNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlEllipsoidNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                ellipsoid->setMaterial(parseMaterial(node));
            else if (equals(node->name, "transformation"))
                ellipsoid->setTransformation(parseTransformation(node));
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlEllipsoidNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return ellipsoid;
}

std::shared_ptr<GMesh> XMLParser::parseMesh(xmlNode *xmlMeshNode)
{
    if(xmlMeshNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Box, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return nullptr;
    }

    const xmlAttr *attr = NULL;
    std::string name(""), src(""), mat("");

    for (attr = xmlMeshNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "src"))
        {
            src = (const char*)attr->children->content;
        }
        else if (equals(attr->name, "material"))
        {
            mat = (const char*)attr->children->content;
        }
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlMeshNode->name << "\':" << name << "\x1b[0m" << std::endl;
    }

    auto mesh = Resource::Get<Mesh>(src);

    if(!mesh)
    {
        mesh = Resource::Create<Mesh>(src);
        OBJParser::ParseMesh(mesh, src);
    }

    auto gmesh = std::shared_ptr<GMesh>(new GMesh(mesh));

    auto material = Material::Get(mat);
    //if(!material) LogError(xmlMeshNode, attr, "Can't find material");
    gmesh->setMaterial(material);

    xmlNode *node = NULL;
    for (node = xmlMeshNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                gmesh->setMaterial(parseMaterial(node));
            else if (equals(node->name, "transformation"))
                gmesh->setTransformation(parseTransformation(node));
            else
                std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlMeshNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return gmesh;
}

Matrix4 XMLParser::parseTransformation(xmlNode *xmlTrnasformationNode)
{
    if(xmlTrnasformationNode == NULL)
    {
         std::cerr << "\x1b[33;1m" << "error: could not parse Model, xmlNode pointer is NULL" << "\x1b[0m" << std::endl;
         return Matrix4();
    }

    const xmlAttr *attr = NULL;
    Vector3 translate(0.0f), rotate(0.0f), scale(1.0f);
    for (attr = xmlTrnasformationNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "translate"))
            translate = toVector(attr->children->content);
        else if (equals(attr->name, "rotate"))
        {
            rotate = toVector(attr->children->content);
            rotate.x = deg2rad(rotate.x);
            rotate.y = deg2rad(rotate.y);
            rotate.z = deg2rad(rotate.z);
        }
        else if (equals(attr->name, "scale"))
            scale = toVector(attr->children->content);
        else
            std::cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlTrnasformationNode->name << "\'" << "\x1b[0m" << std::endl;
    }

    xmlNode *node = NULL;
    for (node = xmlTrnasformationNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            std::cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlTrnasformationNode->name << "\'" << "\x1b[0m" << std::endl;
        }
    }

    return Transformation::TSR(translate, rotate, scale);
}
