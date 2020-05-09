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
    std::shared_ptr<Texture>  texture;

    const std::string& getName() const { return name;}


    static std::shared_ptr<Material> Create(std::string name, Type type);
    static std::shared_ptr<Material> GetByName(const std::string &name);

    static const std::shared_ptr<Material> DiffuseWhite;
    static const std::shared_ptr<Material> Glass;
    static const std::shared_ptr<Material> Mirror;

protected:
    Material(std::string name, Type type);
    std::string name;
    static std::map<std::string, std::shared_ptr<Material>> MaterialList;

};

class MaterialPossessor
{
protected:
    std::shared_ptr<Material> mat1 = Material::DiffuseWhite;

public:
    std::shared_ptr<Material> getMat1() const;
    void setMat1(const std::shared_ptr<Material> &value);
};

#endif // MATERIAL_H
