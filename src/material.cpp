#include "material.h"

std::map<std::string, std::shared_ptr<Material>> Material::MaterialList;

const std::shared_ptr<Material> Material::DiffuseWhite = Material::Create("DiffuseWhite", Material::Type::DIFFUSE);
const std::shared_ptr<Material> Material::Glass = Material::Create("Glass", Material::Type::TRANSPARENT);
const std::shared_ptr<Material> Material::Mirror = Material::Create("Mirror", Material::Type::SPECULAR);

std::shared_ptr<Material> Material::Create(std::string name, Type type)
{
    std::shared_ptr<Material> m (new Material(name, type));
    MaterialList[m->getName()] = m;
    return m;
}

std::shared_ptr<Material> Material::GetByName(const std::string &name)
{
    auto m = MaterialList.find(name);
    return m == MaterialList.end() ? nullptr : m->second;
}

Material::Material(std::string name, Type type)
{

    this->name = name;
    Kd = 1.0f;
    E = 0.0f;
    Ks = 0.0f;
    Ns = 30.0f;
    R0 = 0.9f;
    Ni = 1.55f; // refractive index for glass
    this->type = type;
    texture = nullptr;
}

std::shared_ptr<Material> MaterialPossessor::getMat1() const
{
    return mat1;
}

void MaterialPossessor::setMat1(const std::shared_ptr<Material> &value)
{
    mat1 = value ? value : Material::DiffuseWhite;
}
