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
public:
    enum class Type { DIFFUSE, SPECULAR, TRANSPARENT };
    Type type;
    Vector3 Kd;
    Vector3 E;
    float Ks;
    float Ns;
    float R0;
    float Ni;

    const std::string& getName() const { return name;}

    const Texture * getTexture() const;
    void setTexture(const std::shared_ptr<Texture> texture);
    void setTexture(const std::string &name);

    static std::shared_ptr<Material> Create(std::string name="", Type type=Type::DIFFUSE);
    static std::shared_ptr<Material> GetByName(const std::string &name);

    static const std::shared_ptr<Material> DiffuseWhite;
    static const std::shared_ptr<Material> Glass;
    static const std::shared_ptr<Material> Mirror;

protected:
    Material(std::string name, Type type=Type::DIFFUSE);
    std::string name;
    std::shared_ptr<Texture>  texture;

    static std::map<std::string, std::shared_ptr<Material>> MaterialList;

};

#endif // MATERIAL_H
