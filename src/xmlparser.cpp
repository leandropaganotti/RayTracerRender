#include "xmlparser.h"

bool XMLParser::equals(const xmlChar *lhs, const char *rhs)
{
    unsigned int sz = strlen( (const char*) lhs);

    if (strlen(rhs) != sz) return false;

    for (unsigned i = 0; i < sz; ++i)
        if (tolower(lhs[i]) != tolower(rhs[i]))
            return false;
    return true;
}

void XMLParser::error(const xmlNode *node)
{
    if (node && node->parent)
        cerr << "Error parsing \"" << node->name << "\" inside \"" << node->parent->name << "\"" << endl;
    else
        cerr << "Error parsing xml file" << endl;
}

Vector3f XMLParser::toVector(const xmlNode *node)
{
    if (node == NULL) return Vector3f(0.0f);
    return toVector(node->content);
}

Vector3f XMLParser::toVector(const xmlAttr *attr)
{
    if (attr == NULL) return Vector3f(0.0f);
    return toVector(attr->children->content);
}

Vector3f XMLParser::toVector(const xmlChar *str)
{
    if (str == NULL) return Vector3f(0.0f);

    string text((const char*)str);
    size_t first = text.find_first_of(',');

    // if Vector is only one float : Vector3f(x)
    if (first == string::npos)
    {
        return Vector3f(stof(text));
    }
    // if Vector is 3 floats: Vector3f(x, y, z)
    else
    {
        Vector3f vec;
        vec.x = stof(text.substr(0, first));
        text.erase(0, first+1);
        size_t second = text.find_first_of(',');
        if (second == string::npos)
        {

            return Vector3f(0.0f);
        }
        else
        {
            vec.y = stof(text.substr(0, second));
            text.erase(0, second+1);
            if (text.size() == 0)
            {

                return Vector3f(0.0f);
            }
            else
                vec.z = stof(text);
        }
        return vec;
    }
}

float XMLParser::toFloat(const xmlNode *node)
{
    return node == NULL ? 0.0 : atof((const char*)node->content);
}

float XMLParser::toFloat(const xmlChar *str)
{
    return str == NULL ? 0.0 : atof((const char*)str);
}

int XMLParser::toInt(const xmlNode *node)
{
    return node == NULL ? 0 : atoi((const char*)node->content);
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
        cerr << "error: could not parse file " << filename << endl;
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
    	cerr << "error: could not parse Scene, xmlNode pointer is NULL" << endl;
    	return;
    }

    if(xmlSceneNode->properties && equals(xmlSceneNode->properties->name, "id"))
        scene.id.assign((const char*)xmlSceneNode->properties->children->content);

    xmlNode *node = NULL;
    for (node = xmlSceneNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "CameraOptions"))
                parseCameraOptions(node, scene.cameraOptions);
            else if (equals(node->name, "Sphere"))
            {
                Sphere *sphere = new Sphere();
                parseSphere(node, *sphere);
                scene.addObject(sphere);
            }
            else if(equals(node->name, "ambientIndex"))
               scene.ambientIndex = toFloat(node->children);
            else if(equals(node->name, "PointLight"))
            {
                PointLight *light = new PointLight();
                parsePointLight(node, *light);
                scene.addLight(light);
            }
            else if(equals(node->name, "kAmbient"))
                scene.kAmbient = toFloat(node->children);
            else if(equals(node->name, "plane"))
            {
                Plane *plane = new Plane();
                parsePlane(node, *plane);
                scene.addObject(plane);
            }
            else
                error(node);
        }
    }
}

void XMLParser::parseCameraOptions(xmlNode *xmlCameraOptionsNode, CameraOptions & options)
{
    if(xmlCameraOptionsNode == NULL)
    {
	    cerr << "error: could not parse CameraOptions, xmlNode pointer is NULL" << endl;
	    return;
    }
    xmlNode *node = NULL;
    for (node = xmlCameraOptionsNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "position"))
                options.setFrom( toVector(node->children) );
            else if (equals(node->name, "lookingAt"))
                options.setTo( toVector(node->children) );
            else if (equals(node->name, "fov"))
                options.setFov( deg2rad(toFloat(node->children) ));
            else if (equals(node->name, "width"))
                options.setWidth( toInt(node->children) );
            else if (equals(node->name, "height"))
                options.setHeight( toInt(node->children) );
            else
                error(node);
        }
    }
}

void XMLParser::parseSphere(xmlNode *xmlSphereNode, Sphere & sphere)
{
    if(xmlSphereNode == NULL)
    {
	    cerr << "error: could not parse Sphere, xmlNode pointer is NULL" << endl;
	    return;
    }
    xmlNode *node = NULL;
    for (node = xmlSphereNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "center"))
                sphere.setCenter( toVector(node->children) );
            else if (equals(node->name, "radius"))
                sphere.setRadius( toFloat(node->children) );
            else if (equals(node->name, "material"))
                parseMaterial(node, sphere.material);
            else if (equals(node->name, "texture"))
                parseTexture(node, sphere.getTex());
            else
                error(node);
        }
    }
}

void XMLParser::parseMaterial(xmlNode *xmlMaterialNode, Material & material)
{
    if(xmlMaterialNode == NULL)
    {
         cerr << "error: could not parse Material, xmlNode pointer is NULL" << endl;
         return;
    }
    xmlNode *node = NULL;
    for (node = xmlMaterialNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "kDiffuse"))
                material.kDiffuse = toVector(node->children);
            else if (equals(node->name, "kSpecular"))
                material.kSpecular = toVector(node->children);
            else if (equals(node->name, "shininess"))
                material.shininess = toFloat(node->children);
            else if (equals(node->name, "reflectivity"))
                material.reflectivity = toFloat(node->children);
            else if (equals(node->name, "refractiveIndex"))
                material.refractiveIndex = toFloat(node->children);
            else if (equals(node->name, "type"))
            {
                if (equals(node->children->content, "DIFFUSE"))
                    material.type = Material::Type::DIFFUSE;
                else if (equals(node->children->content, "SPECULAR"))
                    material.type = Material::Type::SPECULAR;
                else if (equals(node->children->content, "MIRROR"))
                    material.type = Material::Type::MIRROR;
                else if (equals(node->children->content, "TRANSPARENT"))
                    material.type = Material::Type::TRANSPARENT;
                else
                    error(node);
            }
            else
                error(node);
        }
    }
}

void XMLParser::parsePointLight(xmlNode *xmlPointLightNode, PointLight & light)
{
    if(xmlPointLightNode == NULL)
    {
         cerr << "error: could not parse PointLight, xmlNode pointer is NULL" << endl;
         return;
    }
    xmlNode *node = NULL;
    for (node = xmlPointLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "position"))
                light.setPos( toVector(node->children) );
            else if (equals(node->name, "color"))
                light.setColor( toVector(node->children) );
            else if (equals(node->name, "strength"))
                light.setStrength( toFloat(node->children) );
            else if (equals(node->name, "k"))
                light.setK( toFloat(node->children) );
            else
                error(node);
        }
    }
}

void XMLParser::parsePlane(xmlNode *xmlPlaneNode, Plane & plane)
{
    if(xmlPlaneNode == NULL)
    {
         cerr << "error: could not parse Plane, xmlNode pointer is NULL" << endl;
         return;
    }
    xmlNode *node = NULL;
    for (node = xmlPlaneNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "point"))
                plane.O = toVector(node->children);
            else if (equals(node->name, "normal"))
                plane.N = toVector(node->children);
            else if (equals(node->name, "material"))
                parseMaterial(node, plane.material);
            else if (equals(node->name, "texture"))
                parseTexture(node, plane.getTex());
            else
                error(node);
        }
    }
}

void XMLParser::parseTexture(xmlNode *xmlTextureNode, std::unique_ptr<Texture> &tex)
{
    if(xmlTextureNode == NULL)
    {
         cerr << "error: could not parse Texture, xmlNode pointer is NULL" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
    xmlChar *name=NULL;
    float rows=1.0f, cols=1.0f, uedge=0.1f, vedge=0.1f, angle=0.0f;
    Vector3f color1=0, color2=1;
    for (attr = xmlTextureNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = attr->children->content;
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
            error(xmlTextureNode);
    }
    if(name)
    {
        if (equals(name, "tiles"))
            tex.reset(new Tiles(color1, color2, rows, cols, angle, uedge, vedge));
        else if (equals(name, "chessBoard"))
            tex.reset(new ChessBoard(color1, color2, rows, cols, angle));
    }
    else
        std::cerr << "Error parsing Texture type attribute not found in " << xmlTextureNode->name << std::endl;

}
