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

Vector3 XMLParser::toVector(const xmlChar *str)
{
    if (str == NULL) return Vector3(0.0f);

    string text((const char*)str);
    size_t first = text.find_first_of(',');

    // if Vector is only one float : Vector3(x)
    if (first == string::npos)
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
        if (second == string::npos)
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
        cerr << "\x1b[33;1m" << "error: could not parse file " << filename << "\x1b[0m" << endl;
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
        cerr << "\x1b[33;1m" << "error: could not parse Scene, xmlNode pointer is NULL" << "\x1b[0m" << endl;
    	return;
    }

    const xmlAttr *attr = NULL;
    string name("");
    for (attr = xmlSceneNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "spp"))
             scene.spp = toInt(attr->children->content);
        else if (equals(attr->name, "grid"))
             scene.grid = toInt(attr->children->content);
        else if (equals(attr->name, "index"))
             scene.ambientIndex = toFloat(attr->children->content);
        else if (equals(attr->name, "maxdepth"))
             scene.maxDepth = toInt(attr->children->content);
        else if (equals(attr->name, "bgcolor"))
             scene.bgColor = toVector(attr->children->content);
        else if (equals(attr->name, "ka"))
             scene.ka = toFloat(attr->children->content);
        else if (equals(attr->name, "raytracer"))
        {
            if (equals(attr->children->content, "path"))
                scene.raytracer = RayTracerType::PathTracer;
            else if (equals(attr->children->content, "path2"))
                scene.raytracer = RayTracerType::PathTracerWithDirectLightSampling;
            else
                scene.raytracer = RayTracerType::Phong;
        }
        else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlSceneNode->name << "\':" << name << "\x1b[0m" << endl;
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
            else if (equals(node->name, "Sphere"))
            {
                Sphere *sphere = new Sphere();
                parseSphere(node, *sphere);
                scene.addObject(sphere);
            }
            else if(equals(node->name, "ambientIndex"))
               scene.ambientIndex = toFloat(node->children->content);
            else if(equals(node->name, "PointLight"))
            {
                PointLight *light = new PointLight();
                parsePointLight(node, *light);
                scene.addLight(light);
            }
            else if(equals(node->name, "DistantLight"))
            {
                DistantLight *light = new DistantLight();
                parseDistantLight(node, *light);
                scene.addLight(light);
            }
            else if(equals(node->name, "kAmbient"))
                scene.ka = toFloat(node->children->content);
            else if(equals(node->name, "plane"))
            {
                Plane *plane = new Plane();
                parsePlane(node, *plane);
                scene.addObject(plane);
            }
            else if(equals(node->name, "box"))
            {
                Box *box = new Box();
                parseBox(node, *box);
                scene.addObject(box);
            }
            else if(equals(node->name, "model"))
            {
                OBJModel *model = new OBJModel();
                parseModel(node, *model);
                scene.addObject(model);
            }
            else
                cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlSceneNode->name << "\'" << "\x1b[0m" << endl;
        }
    }    
}

void XMLParser::parseCameraOptions(xmlNode *xmlCameraOptionsNode, CameraOptions & options)
{
    if(xmlCameraOptionsNode == NULL)
    {
        cerr << "\x1b[33;1m" << "error: could not parse CameraOptions, xmlNode pointer is NULL" << "\x1b[0m" << endl;
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
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlCameraOptionsNode->name << "\'" << "\x1b[0m" << endl;
	}

    xmlNode *node = NULL;
    for (node = xmlCameraOptionsNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlCameraOptionsNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parseSphere(xmlNode *xmlSphereNode, Sphere & sphere)
{
    if(xmlSphereNode == NULL)
    {
        cerr << "\x1b[33;1m" << "error: could not parse Sphere, xmlNode pointer is NULL" << "\x1b[0m" << endl;
	    return;
    }

    const xmlAttr *attr = NULL;
    string name("");
    for (attr = xmlSphereNode->properties; attr; attr = attr->next)
	{
    	if (equals(attr->name, "name"))
    		name = (const char*)attr->children->content;
    	else if (equals(attr->name, "position"))
			sphere.setCenter( toVector(attr->children->content) );
		else if (equals(attr->name, "radius"))
		    sphere.setRadius( toFloat(attr->children->content) );
        else if (equals(attr->name, "material"))
            sphere.setMaterial((const char*)attr->children->content);
        else if (equals(attr->name, "texture"))
            sphere.setTexture((const char*)attr->children->content);
		else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlSphereNode->name << "\':" << name << "\x1b[0m" << endl;
	}

    xmlNode *node = NULL;
    for (node = xmlSphereNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
        	if (equals(node->name, "material"))
            {                
                std::shared_ptr<Material> material;
                parseMaterial(node, material);
                sphere.setMaterial(material);
            }
            else if (equals(node->name, "texture"))
            {                
                std::shared_ptr<Texture> tex;
                parseTexture(node, tex);
                sphere.setTexture(tex);
            }
            else
                cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlSphereNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parseMaterial(xmlNode *xmlMaterialNode, std::shared_ptr<Material> &material)
{    
    if(xmlMaterialNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse Material, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    xmlChar *attName = xmlGetProp(xmlMaterialNode,(const xmlChar*)"name");
    material = Material::Create(attName? (const char*)attName : "");
    xmlFree(attName);
    const xmlAttr *attr = NULL;
    string name = "";
    for (attr = xmlMaterialNode->properties; attr; attr = attr->next)
	{
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "kd"))
            material->kd = toVector(attr->children->content);
        else if (equals(attr->name, "E"))
            material->E = toVector(attr->children->content);
        else if (equals(attr->name, "ks"))
            material->ks = toFloat(attr->children->content);
        else if (equals(attr->name, "m"))
            material->m = toFloat(attr->children->content);
        else if (equals(attr->name, "R0"))
            material->R0 = toFloat(attr->children->content);
        else if (equals(attr->name, "index"))
            material->index = toFloat(attr->children->content);
		else if (equals(attr->name, "type"))
		{
			if (equals(attr->children->content, "DIFFUSE"))
                material->type = Material::Type::DIFFUSE;
			else if (equals(attr->children->content, "SPECULAR"))
                material->type = Material::Type::SPECULAR;
			else if (equals(attr->children->content, "TRANSPARENT"))
                material->type = Material::Type::TRANSPARENT;
			else
                cerr << "\x1b[33;1m" << "unrecognized material type \'" << attr->children->content << "\'" << "\x1b[0m" << endl;
		}
		else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlMaterialNode->name << "\':" << name << "    \x1b[0m" << endl;
	}    
    xmlNode *node = NULL;
    for (node = xmlMaterialNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlMaterialNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parsePointLight(xmlNode *xmlPointLightNode, PointLight & light)
{
    if(xmlPointLightNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse PointLight, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
	string name("");
	for (attr = xmlPointLightNode->properties; attr; attr = attr->next)
	{
		if (equals(attr->name, "name"))
			name = (const char*)attr->children->content;
		else if (equals(attr->name, "position"))
			light.setPos( toVector(attr->children->content) );
		else if (equals(attr->name, "color"))
            light.setColor( toVector(attr->children->content) );
		else if (equals(attr->name, "strength"))
            light.setStrength( toFloat(attr->children->content) );
		else if (equals(attr->name, "k"))
			light.setK( toFloat(attr->children->content) );
		else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlPointLightNode->name << "\':" << name << "\x1b[0m" << endl;
	}

    xmlNode *node = NULL;
    for (node = xmlPointLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlPointLightNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parseDistantLight(xmlNode *xmlDistantLightNode, DistantLight &light)
{
    if(xmlDistantLightNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse PointLight, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
    string name("");
    for (attr = xmlDistantLightNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "direction"))
            light.setDir( toVector(attr->children->content) );
        else if (equals(attr->name, "color"))
            light.setColor( toVector(attr->children->content) );
        else if (equals(attr->name, "strength"))
            light.setStrength( toFloat(attr->children->content) );
        else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlDistantLightNode->name << "\':" << name << "\x1b[0m" << endl;
    }

    xmlNode *node = NULL;
    for (node = xmlDistantLightNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlDistantLightNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parsePlane(xmlNode *xmlPlaneNode, Plane & plane)
{
    if(xmlPlaneNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse Plane, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
	string name("");
	for (attr = xmlPlaneNode->properties; attr; attr = attr->next)
	{
		if (equals(attr->name, "name"))
			name = (const char*)attr->children->content;
		else if (equals(attr->name, "point"))
			plane.O = toVector(attr->children->content);
		else if (equals(attr->name, "normal"))
			plane.N = toVector(attr->children->content);
        else if (equals(attr->name, "material"))
            plane.setMaterial((const char*)attr->children->content);
        else if (equals(attr->name, "texture"))
            plane.setTexture((const char*)attr->children->content);
        else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlPlaneNode->name << "\':" << name << "\x1b[0m" << endl;
	}

    xmlNode *node = NULL;
    for (node = xmlPlaneNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
        	if (equals(node->name, "material"))
            {
                std::shared_ptr<Material> material;
                parseMaterial(node, material);
                plane.setMaterial(material);
            }
            else if (equals(node->name, "texture"))
            {
                std::shared_ptr<Texture> tex;
                parseTexture(node, tex);
                plane.setTexture(tex);
            }
            else
                cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlPlaneNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parseTexture(xmlNode *xmlTextureNode, std::shared_ptr<Texture> &tex)
{
    if(xmlTextureNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse Texture, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
    string name("");
    string type("");
    float rows=1.0f, cols=1.0f, uedge=0.1f, vedge=0.1f, angle=0.0f;
    Vector3 color1=0, color2=1;
    for (attr = xmlTextureNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "type"))
            type = (const char*)attr->children->content;
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
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlTextureNode->name << "\'" << "\x1b[0m" << endl;
    }
    if(type!="")
    {
        if (type == "Tiles")
            tex = Tiles::Create(name, color1, color2, rows, cols, angle, uedge, vedge);
        else if (type == "ChessBoard")
            tex = ChessBoard::Create(name, color1, color2, rows, cols, angle);
        else
            tex = nullptr;
    }
    else
        cerr << "\x1b[33;1m" << "Error parsing Texture type attribute not found in " << xmlTextureNode->name << "\x1b[0m" << endl;

}

void XMLParser::parseBox(xmlNode *xmlBoxNode, Box &box)
{
    if(xmlBoxNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse Box, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
    string name("");
    for (attr = xmlBoxNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "material"))
            box.setMaterial((const char*)attr->children->content);
        else if (equals(attr->name, "texture"))
            box.setTexture((const char*)attr->children->content);
        else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlBoxNode->name << "\':" << name << "\x1b[0m" << endl;
    }

    xmlNode *node = NULL;
    for (node = xmlBoxNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
            {
                std::shared_ptr<Material> material;
                parseMaterial(node, material);
                box.setMaterial(material);
            }
            else if (equals(node->name, "transformation"))
                parseTransformation(node, box);
            else if (equals(node->name, "texture"))
            {
                std::shared_ptr<Texture> tex;
                parseTexture(node, tex);
                box.setTexture(tex);
            }
            else
                cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlBoxNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
}

void XMLParser::parseTransformation(xmlNode *xmlTrnasformationNode, Transformation &transformation)
{
    if(xmlTrnasformationNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse Model, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
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
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlTrnasformationNode->name << "\'" << "\x1b[0m" << endl;
    }

    xmlNode *node = NULL;
    for (node = xmlTrnasformationNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlTrnasformationNode->name << "\'" << "\x1b[0m" << endl;
        }
    }
    transformation.setTransformation(translate, rotate, scale);
}

void XMLParser::parseModel(xmlNode *xmlModelNode, OBJModel &model)
{
    if(xmlModelNode == NULL)
    {
         cerr << "\x1b[33;1m" << "error: could not parse Model, xmlNode pointer is NULL" << "\x1b[0m" << endl;
         return;
    }

    const xmlAttr *attr = NULL;
    std::string name;
    for (attr = xmlModelNode->properties; attr; attr = attr->next)
    {
        if (equals(attr->name, "name"))
            name = (const char*)attr->children->content;
        else if (equals(attr->name, "path"))
            model.loadFromFile((const char*)attr->children->content);
        else if (equals(attr->name, "material"))
            model.setMaterial((const char*)attr->children->content);
        else
            cerr << "\x1b[33;1m" << "unrecognized attribute \'" << attr->name << "\' in element \'" << xmlModelNode->name << "\'" << "\x1b[0m" << endl;
    }

    xmlNode *node = NULL;
    for (node = xmlModelNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE)
        {
            if (equals(node->name, "material"))
            {
                std::shared_ptr<Material> material;
                parseMaterial(node, material);
                model.setMaterial(material);
            }
            else if (equals(node->name, "transformation"))
            {                                
                parseTransformation(node, model);                
            }
            else
            {
                cerr << "\x1b[33;1m" << "unrecognized element \'" << node->name << "\' in element \'" << xmlModelNode->name << "\'" << "\x1b[0m" << endl;
            }
        }
    }    
}
