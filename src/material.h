#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class Material
{
    Material(std::string name);
    std::string name;
    static std::map<std::string, std::shared_ptr<Material>> MaterialList;
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

    static std::shared_ptr<Material> Create(std::string name="");
    static std::shared_ptr<Material> GetByName(const std::string &name);

    static const std::shared_ptr<const Material> DiffuseWhite;
    static const std::shared_ptr<const Material> Glass;
    static const std::shared_ptr<const Material> Mirror;
};

#endif // MATERIAL_H
