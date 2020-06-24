#include "xmlparser.h"
#include <cstring>
#include "utils.h"
#include "objparser.h"
#include "transformation.h"
#include "object.h"

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

Vector3 XMLParser::toVector3(const xmlChar *str)
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

bool XMLParser::toBool(const xmlChar *str)
{
    return equals(str, "true") ? true : false;
}

void XMLParser::LogError(const xmlNode *node, const xmlAttr* attr, const char *msg)
{
    if(node)
    {
        std::stringstream ss;
        ss << "\x1b[33;1m Error: <" << node->name;
        if(attr)
            ss << " " << (const char*)attr->name << "=\'" << (const char*)attr->children->content << "\'";
        ss << ">: " << msg << "\x1b[0m" << std::endl;
        std::cerr << ss.str();
    }
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
    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlSceneNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else
            LogError(xmlSceneNode, attr, "unrecognized attribute");
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
            else if(equals(node->name, "SphericalLight"))
            {
                scene.addLight(parseSphericalLight(node));
            }
            else if (equals(node->name, "sphere"))
            {
                scene.addObject(parseSphere(node));
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
                LogError(node, nullptr, "unrecognized element");
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
            options.setFrom( toVector3(attr->children->content) );
        else if (equals(attr->name, "lookingat"))
            options.setTo( toVector3(attr->children->content) );
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
             options.bgColor = toVector3(attr->children->content);
        else if (equals(attr->name, "illum"))
        {
            if (equals(attr->children->content, "phong"))
                options.illum = Illumination::Phong;
            else if (equals(attr->children->content, "global"))
                options.illum = Illumination::GlobalIlumination;
            else if (equals(attr->children->content, "global2"))
                options.illum = Illumination::GlobalIluminationWithDirectSampling;
            else
                LogError(xmlRenderOptionsNode, attr, "unrecognized illum attribute value");
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
            material->Kd = toVector3(attr->children->content);
        else if (equals(attr->name, "ka"))
            material->Ka = toVector3(attr->children->content);
        else if (equals(attr->name, "E"))
            material->E = toVector3(attr->children->content);
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

std::shared_ptr<Light> XMLParser::parsePointLight(xmlNode *xmlPointLightNode)
{
    ParamSet params;
    const xmlAttr *attr = NULL;
    for (attr = xmlPointLightNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            params.set<std::string>((const char*)attr->name, (const char*)attr->children->content);
        else if (equals(attr->name, "shadowoff"))
            params.set<bool>((const char*)attr->name, toBool(attr->children->content));
        else if (equals(attr->name, "position"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content));
        else if (equals(attr->name, "color"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content) );
        else if (equals(attr->name, "intensity"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content) );
        else if (equals(attr->name, "atten"))
            params.set<float>((const char*)attr->name, toFloat(attr->children->content) );
        else
            LogError(xmlPointLightNode, attr, "unrecognized attribute");
    }

    auto light = Light::Create(LightType::PointLight, params);

    xmlNode *node = NULL;
    for (node = xmlPointLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            LogError(node, nullptr, "unrecognized element");
        }
    }
    return light;
}

std::shared_ptr<Light> XMLParser::parseDistantLight(xmlNode *xmlDistantLightNode)
{
    ParamSet params;
    const xmlAttr *attr = NULL;
    for (attr = xmlDistantLightNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            params.set<std::string>((const char*)attr->name, (const char*)attr->children->content);
        else if (equals(attr->name, "shadowoff"))
            params.set<bool>((const char*)attr->name, toBool(attr->children->content));
        else if (equals(attr->name, "direction"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content));
        else if (equals(attr->name, "color"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content) );
        else if (equals(attr->name, "intensity"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content) );
        else
            LogError(xmlDistantLightNode, attr, "unrecognized attribute");
    }

    auto light = Light::Create(LightType::DistantLight, params);

    xmlNode *node = NULL;
    for (node = xmlDistantLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            LogError(node, nullptr, "unrecognized element");
        }
    }
    return light;
}

std::shared_ptr<Light> XMLParser::parseSphericalLight(xmlNode *xmlsSphericalLightNode)
{
    ParamSet params;
    const xmlAttr *attr = NULL;
    for (attr = xmlsSphericalLightNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            params.set<std::string>((const char*)attr->name, (const char*)attr->children->content);
        else if (equals(attr->name, "shadowoff"))
            params.set<bool>((const char*)attr->name, toBool(attr->children->content));
        else if (equals(attr->name, "position"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content));
        else if (equals(attr->name, "color"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content) );
        else if (equals(attr->name, "intensity"))
            params.set<Vector3>((const char*)attr->name, toVector3(attr->children->content) );
        else if (equals(attr->name, "atten"))
            params.set<float>((const char*)attr->name, toFloat(attr->children->content) );
        else if (equals(attr->name, "radius"))
            params.set<float>((const char*)attr->name, toFloat(attr->children->content) );
        else
            LogError(xmlsSphericalLightNode, attr, "unrecognized attribute");
    }

    auto light = Light::Create(LightType::SphericalLight, params);

    xmlNode *node = NULL;
    for (node = xmlsSphericalLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            LogError(node, nullptr, "unrecognized element");
        }
    }
    return light;
}

std::shared_ptr<Object> XMLParser::parsePlane(xmlNode *xmlPlaneNode)
{
    auto plane = std::shared_ptr<Plane>( new Plane);
    std::shared_ptr<Material> material;
    Matrix4 transform;

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlPlaneNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "origin"))
            plane->setOrigin(toVector3(attr->children->content));
        else if (equals(attr->name, "normal"))
            plane->setNormal(toVector3(attr->children->content));
        else if (equals(attr->name, "material"))
            material = Material::Get((const char*)attr->children->content);
        else
            LogError(xmlPlaneNode, attr, "unrecognized attribute");
    }

    xmlNode *node = NULL;
    for (node = xmlPlaneNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                material = parseMaterial(node);
            else if(equals(node->name, "transformation"))
                transform = parseTransformation(node);
            else
                LogError(node, nullptr, "unrecognized element");
        }
    }

    return std::shared_ptr<Object>(new TransformedObject(plane, material, transform));
}

std::shared_ptr<Object> XMLParser::parseSphere(xmlNode * xmlSphereNode)
{
    auto sphere = std::shared_ptr<Sphere>(new Sphere);
    std::shared_ptr<Material> material;
    Matrix4 transform;

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlSphereNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "position"))
            sphere->setCenter( toVector3(attr->children->content) );
        else if (equals(attr->name, "radius"))
            sphere->setRadius( toFloat(attr->children->content) );
        else if (equals(attr->name, "material"))
        {
            material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlSphereNode, attr, "Can't find material");
        }
        else
            LogError(xmlSphereNode, attr, "unrecognized attribute");
    }

    xmlNode *node = NULL;
    for (node = xmlSphereNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                material = parseMaterial(node);
            else if (equals(node->name, "transformation"))
                transform = parseTransformation(node);
            else
                LogError(node, nullptr, "unrecognized element");
        }
    }

    return std::shared_ptr<Object>(new TransformedObject(sphere, material, transform));
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
    float rows=1.0f, cols=1.0f, uedge=0.1f, vedge=0.1f, angle=0.0f, width=1.0f, height=1.0f;
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
        else if (equals(attr->name, "width"))
            width = toFloat(attr->children->content);
        else if (equals(attr->name, "height"))
            height = toFloat(attr->children->content);
        else if (equals(attr->name, "uedge"))
            uedge = toFloat(attr->children->content);
        else if (equals(attr->name, "vedge"))
            vedge = toFloat(attr->children->content);
        else if (equals(attr->name, "color1"))
            color1 = toVector3(attr->children->content);
        else if (equals(attr->name, "color2"))
            color2 = toVector3(attr->children->content);
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
            tex->setWidth(width);
            tex->setHeight(height);
        }
        return tex;
    }

    std::cerr << "\x1b[33;1m" << "Error parsing Texture, \'Type\' not found or invalid in " << xmlTextureNode->name << "\x1b[0m" << std::endl;
    return nullptr;
}

std::shared_ptr<Object> XMLParser::parseBox(xmlNode *xmlBoxNode)
{
    const xmlAttr *attr = NULL;
    std::string name("");
    std::shared_ptr<Material> material;
    for (attr = xmlBoxNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "material"))
        {
            material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlBoxNode, attr, "Can't find material");
        }
        else
            LogError(xmlBoxNode, attr, "unrecognized attribute");
    }

    Matrix4 transform;
    xmlNode *node = NULL;
    for (node = xmlBoxNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                material = parseMaterial(node);
            else if (equals(node->name, "transformation"))
                transform = parseTransformation(node);
            else
                LogError(node, nullptr, "unrecognized element");
        }
    }
    auto box = std::shared_ptr<AABox>(new AABox);
    return std::shared_ptr<Object>(new TransformedObject(box, material, transform));
}

std::shared_ptr<Object> XMLParser::parseCylinder(xmlNode *xmlCylinderNode)
{
    auto cylinder = std::shared_ptr<UnitYCylinder>(new UnitYCylinder);
    std::shared_ptr<Material> material;

    const xmlAttr *attr = NULL;
    std::string name("");
    for (attr = xmlCylinderNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "material"))
        {
            material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlCylinderNode, attr, "Can't find material");
        }
        else
            LogError(xmlCylinderNode, attr, "unrecognized attribute");
    }

    Matrix4 transform;
    xmlNode *node = NULL;
    for (node = xmlCylinderNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                material = parseMaterial(node);
            else if (equals(node->name, "transformation"))
                transform = parseTransformation(node);
            else
                LogError(node, nullptr, "unrecognized element");
        }
    }

    return std::shared_ptr<Object>(new TransformedObject(cylinder, material, transform));
}

std::shared_ptr<Object> XMLParser::parseMesh(xmlNode *xmlMeshNode)
{
    const xmlAttr *attr = NULL;
    std::string name(""), src("");
    std::shared_ptr<Material> material;

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
            material = Material::Get((const char*)attr->children->content);
            if(!material) LogError(xmlMeshNode, attr, "Can't find material");
        }
        else
            LogError(xmlMeshNode, attr, "unrecognized attribute");
    }

    auto mesh = Resource::Get<Mesh>(src);

    if(!mesh)
    {
        mesh = Resource::Create<Mesh>(src);
        OBJParser::ParseMesh(mesh, src);
    }

    Matrix4 transform;

    xmlNode *node = NULL;
    for (node = xmlMeshNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
                material = parseMaterial(node);
            else if (equals(node->name, "transformation"))
                transform = parseTransformation(node);
            else
                LogError(node, nullptr, "unrecognized element");
        }
    }
    mesh->setMaterial(material);
    return std::shared_ptr<Object>(new TransformedObject(mesh, transform));
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
            translate = toVector3(attr->children->content);
        else if (equals(attr->name, "rotate"))
        {
            rotate = toVector3(attr->children->content);
            rotate.x = deg2rad(rotate.x);
            rotate.y = deg2rad(rotate.y);
            rotate.z = deg2rad(rotate.z);
        }
        else if (equals(attr->name, "scale"))
            scale = toVector3(attr->children->content);
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
