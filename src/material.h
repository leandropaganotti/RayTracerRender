#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include "texture.h"
#include "resource.h"

#include <vector>

struct Material;

enum class MaterialType { DIFFUSE, SPECULAR, TRANSPARENT };

namespace material
{
    extern std::shared_ptr<Material> DiffuseWhite;
    extern std::shared_ptr<Material> Glass;
    extern std::shared_ptr<Material> Mirror;
}

struct Material
{
    Material();
    Material(const std::string &name, MaterialType type);
    ~Material();

    static std::shared_ptr<Material> Create();
    static std::shared_ptr<Material> CreateResource(const std::string &name, MaterialType type);

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
