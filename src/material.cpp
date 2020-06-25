#include "material.h"

namespace material
{
    std::shared_ptr<Material> DiffuseWhite = Material::CreateResource("DiffuseWhite", MaterialType::DIFFUSE);
    std::shared_ptr<Material> Glass = Material::CreateResource("Glass", MaterialType::TRANSPARENT);
    std::shared_ptr<Material> Mirror = Material::CreateResource("Mirror", MaterialType::SPECULAR);
}

std::shared_ptr<Material> Material::CreateResource(const std::string &name, MaterialType type)
{
    std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material(name, type));
    Resource<Material>::Add(name, material);
    return material;
}

Material::~Material(){}

std::shared_ptr<Material> Material::Create()
{
    return std::shared_ptr<Material>(new Material);
}

Material::Material(const std::string &name, MaterialType type)
{
    this->name = name;
    Kd = 1.0f;
    Ka = 0.1f;
    E = 0.0f;
    Ks = 0.0f;
    Ns = 30.0f;
    R0 = 0.98f;
    Ni = 1.55f; // refractive index for glass
    this->type = type;
    texture = nullptr;
}

Material::Material()
{
    name = "";
    Kd = 1.0f;
    Ka = 0.1f;
    E = 0.0f;
    Ks = 0.0f;
    Ns = 30.0f;
    R0 = 0.98f;
    Ni = 1.55f; // refractive index for glass
    type = MaterialType::DIFFUSE;
    texture = nullptr;
}


