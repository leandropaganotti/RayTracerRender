#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include "texture.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

class Material
{
    Material(std::string name);
    std::string name;
    static std::map<std::string, std::shared_ptr<Material>> MaterialList;

    std::shared_ptr<Texture>  texture;

public:
    enum class Type { DIFFUSE, SPECULAR, TRANSPARENT };
    Type type;
    Vector3 kd;
    Vector3 E;
    float ks;
    float m;
    float R0;
    float index;    

    const std::string& getName() const { return name;}

    const Texture * getTexture() const;
    void setTexture(const std::shared_ptr<Texture> texture);
    void setTexture(const std::string &name);

    static std::shared_ptr<Material> Create(std::string name="");
    static std::shared_ptr<Material> GetByName(const std::string &name);

    static const std::shared_ptr<Material> DiffuseWhite;
    static const std::shared_ptr<Material> Glass;
    static const std::shared_ptr<Material> Mirror;

};

#endif // MATERIAL_H
