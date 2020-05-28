#include "material.h"

namespace material
{
    const std::shared_ptr<Material> DiffuseWhite = Material::Create("DiffuseWhite", MaterialType::DIFFUSE);
    const std::shared_ptr<Material> Glass = Material::Create("Glass", MaterialType::TRANSPARENT);
    const std::shared_ptr<Material> Mirror = Material::Create("Mirror", MaterialType::SPECULAR);
}

std::shared_ptr<Material> Material::Create(const std::string &key, MaterialType type)
{
    return Resource::Create<Material>(key, new Material(type));
}
std::shared_ptr<Material> Material::Create(const std::string &key)
{
    return Resource::Create<Material>(key, new Material);
}

std::shared_ptr<Material> Material::Get(const std::string &key)
{
    return Resource::Get<Material>(key);
}

Material::~Material(){}

Material::Material(MaterialType type)
{
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


