#include "material.h"

std::map<std::string, std::shared_ptr<Material>> Material::MaterialList;

const std::shared_ptr<const Material> Material::DiffuseWhite = Material::Create("DiffuseWhite");
const std::shared_ptr<const Material> Material::Glass = Material::Create("Glass");
const std::shared_ptr<const Material> Material::Mirror = Material::Create("Mirror");

std::shared_ptr<Material> Material::Create(std::string name)
{
    std::shared_ptr<Material> m (new Material(name=="" ? std::to_string(MaterialList.size()) : name));
    MaterialList[m->getName()] = m;
    return m;
}

std::shared_ptr<Material> Material::GetByName(const std::string &name)
{
    auto m = MaterialList.find(name);
    return m == MaterialList.end() ? nullptr : m->second;
}

Material::Material(std::string name)
{

    this->name = name;
    kd = 1.0f;
    E = 0.0f;
    ks = 0.0f;
    m = 30.0f;
    R0 = 8.0f;
    index = 1.55f; // refractive index for glass
    type = Type::DIFFUSE;
}
