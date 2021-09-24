#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include "texture.h"

#include <vector>

struct Material;

enum class MaterialType { DIFFUSE, SPECULAR, TRANSPARENT, LIGHT };

namespace material
{
    extern std::shared_ptr<Material> DiffuseWhite;
    extern std::shared_ptr<Material> Glass;
    extern std::shared_ptr<Material> Mirror;
}

struct Material
{
    Material(const std::string &name="", MaterialType type=MaterialType::DIFFUSE);
    ~Material();

    static std::shared_ptr<Material> Create();
    static std::shared_ptr<Material> Create(MaterialType type);
    static std::shared_ptr<Material> Create(const std::string &name, MaterialType type);

    std::string name;
    MaterialType type;
    Vector3 Kd;
    Vector3 Ka;
    Vector3 E;
    float Ks;
    float Ns;
    float R0;
    float Ni;
    std::shared_ptr<Texture>  texture;
};

#endif // MATERIAL_H
