#include "material.h"

std::map<std::string, std::shared_ptr<Material>> Material::MaterialList;

const std::shared_ptr<Material> Material::DiffuseWhite = Material::Create("DiffuseWhite", Material::Type::DIFFUSE);
const std::shared_ptr<Material> Material::Glass = Material::Create("Glass", Material::Type::TRANSPARENT);
const std::shared_ptr<Material> Material::Mirror = Material::Create("Mirror", Material::Type::SPECULAR);

std::shared_ptr<Material> Material::Create(std::string name, Type type)
{
    std::shared_ptr<Material> m (new Material(name=="" ? std::to_string(MaterialList.size()) : name, type));
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
    texture = Texture::SolidWhite;
}

const Texture * Material::getTexture() const
{
    return texture.get();
}

void Material::setTexture(const std::shared_ptr<Texture> texture)
{
    this->texture = texture ? texture : Texture::SolidWhite;
}

void Material::setTexture(const std::string &name)
{
    if(!(this->texture = Texture::GetByName(name)))
        this->texture = Texture::SolidWhite;
}
