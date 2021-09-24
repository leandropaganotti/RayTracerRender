#include "material.h"
#include "resource.h"

namespace material
{
    std::shared_ptr<Material> DiffuseWhite = Resource<Material>::Create("DiffuseWhite", MaterialType::DIFFUSE);
    std::shared_ptr<Material> Glass = Resource<Material>::Create("Glass", MaterialType::TRANSPARENT);
    std::shared_ptr<Material> Mirror = Resource<Material>::Create("Mirror", MaterialType::SPECULAR);
}

Material::~Material(){
    std::cout<<"deleted material"<< "\n";
}

std::shared_ptr<Material> Material::Create()
{
    return std::shared_ptr<Material>(new Material);
}

std::shared_ptr<Material> Material::Create(MaterialType type)
{
    return std::shared_ptr<Material>(new Material("", type));
}

std::shared_ptr<Material> Material::Create(const std::string &name, MaterialType type)
{
    return std::shared_ptr<Material>(new Material(name, type));
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


