#pragma once

#include <vector>
#include "vector.h"
#include "texture.h"


class ScatterData
{
public:
    Ray scattered[2];
    Vector3 f[2];
    float pdf[2];
};

enum MaterialType { DIFFUSE, MIRROR, SPECULAR, TRANSPARENT, LIGHT };

struct Material;
class IntersectionData;

namespace material
{
    extern std::shared_ptr<Material> DiffuseWhite;
    extern std::shared_ptr<Material> Glass;
    extern std::shared_ptr<Material> Mirror;
}

class Material
{
public:
    Material(const std::string &name="", MaterialType type=MaterialType::DIFFUSE);
    virtual ~Material() = default;

    static std::shared_ptr<Material> Create(MaterialType type);
    static std::shared_ptr<Material> Create(const std::string &name, MaterialType type);

    std::string name;
    MaterialType type = MaterialType::DIFFUSE;
    Vector3 Kd = 1.0;
    Vector3 Ka = 0.1;
    Vector3 E = 0.0;
    float Ks = 0.0;
    float Ns = 30.0;
    float R0 = 0.98;
    float Ni = 1.55; // refractive index for glass
    std::shared_ptr<Texture>  texture;

    virtual Vector3 emittance(){ return vector::ZERO; }
    virtual void bsdf(const IntersectionData &, ScatterData &) const {}
    virtual Vector3 bsdf(const IntersectionData &, const Vector3 &) const { return vector::ZERO; }

};

class Diffuse: public Material
{
public:
    Diffuse(const std::string& name=""): Material(name, MaterialType::DIFFUSE)
    {

    }
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;

};

class Mirror: public Material{
public:
    Mirror(const std::string& name=""): Material(name, MaterialType::MIRROR)
    {

    }
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;

};

class Transparent: public Material{
public:
    Transparent(const std::string& name=""): Material(name, MaterialType::TRANSPARENT)
    {

    }
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
};
