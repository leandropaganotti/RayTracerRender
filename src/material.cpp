#include "material.h"

const std::shared_ptr<Material> Material::DiffuseWhite = Material::Create("DiffuseWhite", Material::Type::DIFFUSE);
const std::shared_ptr<Material> Material::Glass = Material::Create("Glass", Material::Type::TRANSPARENT);
const std::shared_ptr<Material> Material::Mirror = Material::Create("Mirror", Material::Type::SPECULAR);

std::shared_ptr<Material> Material::Create(const std::string &key, Type type)
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

Material::Material(Material::Type type)
{
    Kd = 1.0f;
    E = 0.0f;
    Ks = 0.0f;
    Ns = 30.0f;
    R0 = 0.9f;
    Ni = 1.55f; // refractive index for glass
    this->type = type;
    texture = nullptr;
}

Material::Material()
{
    Kd = 1.0f;
    E = 0.0f;
    Ks = 0.0f;
    Ns = 30.0f;
    R0 = 0.9f;
    Ni = 1.55f; // refractive index for glass
    type = Type::DIFFUSE;
    texture = nullptr;
}


